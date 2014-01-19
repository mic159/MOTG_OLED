#ifndef MOTG_OLED_H
#define MOTG_OLED_H

#include <Arduino.h>

class MotgOled {
public:
  MotgOled(int cs);
  void begin();
  
  void debugVersion(bool onScreen = true);
  void clearScreen();
  void circle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color);
  void polygon(uint16_t color, uint8_t num, ... );
  void penType(uint8_t type);
  void rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
  void setPixel(uint8_t x, uint8_t y, uint16_t color);
private:
  int cs;
  uint8_t buff[6];
  
  bool read(int bytes);
  void getAck();
};

#endif
