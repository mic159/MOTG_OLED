#include <SPI.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "MOTG_OLED.h"

// The width and height of the display is sent in a short code format
// This table lets us translate that short code into pixel sizes.
PROGMEM prog_uint16_t sizeLookupKey[]   = {0x22, 0x28, 0x32, 0x60, 0x64, 0x76, 0x96};
PROGMEM prog_uint16_t sizeLookupValue[] = { 220,  128,  320,  160,   64,  176,   96};

uint16_t translateToSize(uint8_t data) {
  for (int i = 0; i < 7; ++i)
      if (pgm_read_word_near(sizeLookupKey + i) == data)
        return pgm_read_word_near(sizeLookupValue + i);
  return 0;
}

MotgOled::MotgOled(int _cs)
: cs(_cs)
, width(0)
, height(0)
{}

void MotgOled::begin(bool getResolution) {
  pinMode (cs, OUTPUT);
  digitalWrite(cs,HIGH);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  // On a 16MHz Arduino, setting this lower seems to cause longer commands to fail.
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  
  if (getResolution)
    readDeviceInfo(false, false);
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

void MotgOled::readDeviceInfo(bool onScreen, bool serialEcho) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x56);
  if (onScreen)
    SPI.transfer(0x01);
  else
    SPI.transfer(0x00);
  
  if (read(5) != 0) {
    Serial.println("Error: Timeout");
  } else if (buff[0] == 0x15) {
    Serial.println("Error: Returned NAK (bad command)");
  } else {
    width = translateToSize(buff[3]);
    height = translateToSize(buff[4]);
    if (serialEcho) {
		  Serial.println("**************\n Version Info\n**************");
		  Serial.print("Type: ");
		  Serial.println(buff[0], HEX);
		  Serial.print("Hardware Revision: ");
		  Serial.println(buff[1], HEX);
		  Serial.print("Software Revision: ");
		  Serial.println(buff[2], HEX);
		  Serial.print("Resolution: ");
		  Serial.print(width);
		  Serial.print("x");
		  Serial.println(height);
  		Serial.println("****");
    }
  }
  digitalWrite(cs,HIGH);
}

void MotgOled::clear() {
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

void MotgOled::backgroundColor(uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x4B);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
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

void MotgOled::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x4C);
  SPI.transfer(x1);
  SPI.transfer(y1);
  SPI.transfer(x2);
  SPI.transfer(y2);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x47);
  SPI.transfer(x1);
  SPI.transfer(y1);
  SPI.transfer(x2);
  SPI.transfer(y2);
  SPI.transfer(x3);
  SPI.transfer(y3);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::addUserBitmap(uint8_t num, uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4, uint8_t x5, uint8_t x6, uint8_t x7, uint8_t x8) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x41);
  SPI.transfer(num);
  SPI.transfer(x1);
  SPI.transfer(x2);
  SPI.transfer(x3);
  SPI.transfer(x4);
  SPI.transfer(x5);
  SPI.transfer(x6);
  SPI.transfer(x7);
  SPI.transfer(x8);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::drawUserBitmap(uint8_t num, uint8_t x, uint8_t y, uint16_t color) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x44);
  SPI.transfer(num);
  SPI.transfer(x);
  SPI.transfer(y);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::imageBegin(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colorMode) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x49);
  SPI.transfer(x);
  SPI.transfer(y);
  SPI.transfer(width);
  SPI.transfer(height);
  SPI.transfer(colorMode);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::imageData(uint8_t blockSize, const uint8_t* data) {
  digitalWrite(cs,LOW);
  SPI.transfer(blockSize);
  for (int i = 0; i < blockSize; i++) {
    SPI.transfer(data[i]);
  }
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::imageDataProgmem(uint8_t blockSize, const prog_uchar* data) {
  digitalWrite(cs,LOW);
  SPI.transfer(blockSize);
  for (int i = 0; i < blockSize; i++) {
    SPI.transfer(pgm_read_byte_near(data + i));
  }
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::string(uint8_t col, uint8_t row, Font font, uint16_t color, char * str) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x73);
  SPI.transfer(col);
  SPI.transfer(row);
  SPI.transfer((uint8_t)font);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  for(char* c = str; *c != '\0'; ++c) {
    SPI.transfer(*c);
  }
  SPI.transfer(0);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::string(uint8_t x, uint8_t y, Font font, uint16_t color, uint8_t width, uint8_t height, char * str) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x53);
  SPI.transfer(x);
  SPI.transfer(y);
  SPI.transfer((uint8_t)font);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  SPI.transfer(width);
  SPI.transfer(height);
  for(char* c = str; *c != '\0'; ++c) {
    SPI.transfer(*c);
  }
  SPI.transfer(0);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::opaqueText(bool state) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x4F);
  SPI.transfer(state ? 0x01 : 0x00);
  getAck();
  digitalWrite(cs,HIGH);
}

void MotgOled::font(Font font) {
  digitalWrite(cs,LOW);
  SPI.transfer(0x46);
  SPI.transfer((uint8_t)font);
  getAck();
  digitalWrite(cs,HIGH);
}

