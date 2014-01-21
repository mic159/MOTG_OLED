#ifndef MOTG_OLED_H
#define MOTG_OLED_H

#include <Arduino.h>

class MotgOled {
public:
  MotgOled(int cs);
  void begin(bool getResolution = true);
  
  // Clear the screen. Reset all pixels to the background color.
  void clear();
  
  /*
   * Set the Pen type for shape drawing.
   *
   * @param type: Either solid our outline
   *              0x00 = solid shape
   *              0x01 = shape outline
   */
  void penType(uint8_t type);
  /*
   * Set the baground color.
   * This is used when you clearScreen().
   */
  void backgroundColor(uint16_t color);
  
  void setPixel(uint8_t x, uint8_t y, uint16_t color);
  void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
  void rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
  void circle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color);
  void polygon(uint16_t color, uint8_t num, ... );
  void triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint16_t color);
  
  void addUserBitmap(uint8_t num, uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4, uint8_t x5, uint8_t x6, uint8_t x7, uint8_t x8);
  void drawUserBitmap(uint8_t num, uint8_t x, uint8_t y, uint16_t color);
  
  void imageBegin(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colorMode);
  void imageData(uint8_t blockSize, const uint8_t* data);
  void imageDataProgmem(uint8_t blockSize, const prog_uchar* data);
  
  void readDeviceInfo(bool onScreen = true, bool serialEcho = false);

  // Resolution of the screen.
  // Initialized when you call begin(), or readDeviceInfo().
  int width, height;
private:
  int cs;
  uint8_t buff[6];
  
  bool read(int bytes);
  void getAck();
};

#endif
