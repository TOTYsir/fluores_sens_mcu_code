#ifndef __SSD1306_H
#define __SSD1306_H

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

void lcd_init(void);
void lcd_flush(float volts0, float volts1);

#endif