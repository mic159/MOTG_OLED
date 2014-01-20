#include <SPI.h>
#include <MOTG_OLED.h>

// Screen with CS on pin 10
MotgOled screen(10);

void setup() {
  Serial.begin(57600);
  Serial.println("4D Systems MOTG-OLED Test");
  screen.begin();
}

void loop() {
  delay(1000);
  screen.readDeviceInfo(true);
  delay(1000);
  screen.clearScreen();
  delay(100);
  screen.penType(0);
  screen.circle(screen.width / 2, screen.height / 2, 8, 0x001F /*BLUE*/);
  delay(100);
  screen.polygon(0xF81F /*MAGENTA*/, 7, 16,0, 33,4, 33,25, 15,35, 10,22, 18,15, 8,15);
  delay(100);
  screen.penType(0);
  screen.rectangle(5, 40, 35, 63, 0x07E0 /*LIME*/);
  delay(100);
  screen.penType(1);
  screen.rectangle(40, 40, 70, 63, 0xFFE0 /*YELLOW*/);
  delay(100);
  screen.setPixel(45, 26, 0xFFFF /*WHITE*/);
  delay(100);
  screen.line(5, 15, 60, 0, 0xF81F /*FUCHSIA*/);
  delay(100);
  screen.addUserBitmap(00, 0x18, 0x24, 0x42, 0x81, 0x81, 0x42, 0x24, 0x18);
  screen.addUserBitmap(01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA);
  screen.addUserBitmap(02, 0x20, 0x30, 0x38, 0x3C, 0x3C, 0x38, 0x30, 0x20);
  screen.addUserBitmap(03, 0x01, 0x07, 0x1F, 0x7F, 0x7F, 0x1F, 0x07, 0x01);
  screen.drawUserBitmap(00,  56, 16, 0xF800 /*RED*/);
  delay(100);
  screen.drawUserBitmap(01,  64, 16, 0xFFE0 /*YELLOW*/);
  delay(100);
  screen.drawUserBitmap(02,  72, 16, 0x867F /*LIGHTSKYBLUE*/);
  delay(100);
  screen.drawUserBitmap(03,  80, 16, 0x07FF /*AQUA*/);
  delay(100);
  screen.penType(0);
  screen.triangle(5, 63, 35, 63, 20, 30, 0xF800 /*RED*/);
}
