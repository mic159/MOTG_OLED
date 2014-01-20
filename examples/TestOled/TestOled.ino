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
}
