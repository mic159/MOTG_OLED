#include <SPI.h>
#include <Arduino.h>
#include "MOTG_OLED.h"

MotgOled::MotgOled(int _cs)
: cs(_cs)
{}

void MotgOled::begin() {
  pinMode (cs, OUTPUT);
  digitalWrite(cs,HIGH);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  // On a 16MHz Arduino, setting this lower seems to cause longer commands to fail.
  SPI.setClockDivider(SPI_CLOCK_DIV16);
}

bool MotgOled::read(int expected) {
  int i, j, respc;
  i      = 0;
  respc  = 0;
  buff[respc] = 0xFF;
  while ((i < 300) && (respc != expected)) {
    delay(1); // Give it some time to think
    j = SPI.transfer(0xFF);
    if (j != 0xff)
      buff[respc++] = j;
    i++;
  }
  if (i == 0) {
    return 1;
  } else {
    return 0;
  }
}

void MotgOled::getAck() {
  if (read(1) != 0) {
    // ERROR
    Serial.println("Error: Timeout");
    return;
  }
  if (buff[0] == 0x15) {
    // Error code
    Serial.println("Error: Returned NAK (bad command)");
  } else if (buff[0] != 0x06) {
    // Not success??
    Serial.print("Error: Unexpected value ");
    Serial.println(buff[0], HEX);
  }
}

void MotgOled::debugVersion(bool onScreen) {
  Serial.println("**************\n Version Info\n**************");
  digitalWrite(cs,LOW);
  SPI.transfer(0x56);
  if (onScreen)
    SPI.transfer(0x01);
  else
    SPI.transfer(0x00);
  read(5);
  if (buff[0] == 0x15) {
    Serial.println("Error: Returned NAK (bad command)");
  } else {
    Serial.print("Type: ");
    Serial.println(buff[0], HEX);
    Serial.print("Hardware Revision: ");
    Serial.println(buff[1], HEX);
    Serial.print("Software Revision: ");
    Serial.println(buff[2], HEX);
    Serial.print("Resolution: ");
    Serial.print(0xFF & buff[3], HEX);
    Serial.print("x");
    Serial.println(0xFF & buff[4], HEX);
  }
  Serial.println("****");
  digitalWrite(cs,HIGH);
}

void MotgOled::clearScreen() {
  digitalWrite(cs,LOW);
  SPI.transfer(0x45);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::circle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x43);
  SPI.transfer(x);
  SPI.transfer(y);
  SPI.transfer(radius);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::polygon(uint16_t color, uint8_t num, ... ) {
  va_list arguments;
  if (num < 3 || num > 7) return; // Cant be less than 3 points.
  digitalWrite(cs,LOW);
  SPI.transfer(0x67);
  SPI.transfer(num);
  va_start(arguments, num);
  for (uint8_t i = 0; i < num; ++i) {
    SPI.transfer(va_arg(arguments, int)); // x
    SPI.transfer(va_arg(arguments, int)); // y
  }
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::penType(uint8_t type) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x70);
  SPI.transfer(type);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x72);
  SPI.transfer(x1);
  SPI.transfer(y1);
  SPI.transfer(x2);
  SPI.transfer(y2);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::setPixel(uint8_t x, uint8_t y, uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x50);
  SPI.transfer(x);
  SPI.transfer(y);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

