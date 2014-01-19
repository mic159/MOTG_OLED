#include <MOTG_OLED.h>

// Screen with CS on pin 10
MotgOled screen(10);

void setup() {
  Serial.begin(57600);
  Serial.println("4D Systems MOTG-OLED Test");
}

void loop() {
  delay(1000);
  screen.debugVersion();
  delay(1000);
  screen.clearScreen();
  screen.circle(40,28, 8, 0x001F /*BLUE*/);
  screen.polygon(0xF81F /*MAGENTA*/, 7, 16,0, 33,4, 33,25, 15,35, 10,22, 18,15, 8,15);
  screen.penType(0);
  screen.rectangle(5, 40, 35, 63, 0x07E0 /*LIME*/);
  screen.penType(1);
  screen.rectangle(40, 40, 70, 63, 0xFFE0 /*YELLOW*/);
  screen.setPixel(45, 26, 0xFFFF /*WHITE*/);
}
