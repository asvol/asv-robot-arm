#include "hal.h"

unsigned long previousMillis = 0;     // запоминаем время последнего обновления
const unsigned long interval = 100;   // 100 мс = 0.1 секунды

void setup()
{
  hal_setup();
}

void loop()
{
  unsigned long currentMillis = millis();
  hal_loop();
  int16_t lx = 0;
  int16_t ly = 0;
  int16_t rx = 0;
  int16_t ry = 0;
  hal_joystick_get(lx,ly,rx,ry);
 
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    char buf[16];
    snprintf(buf, sizeof(buf), "%-4d", lx * 100 / 256);
    hal_display_print(0,0,HAL_DISPLAY_FONT_LARGE,buf);

    snprintf(buf, sizeof(buf), "%-4d", ly * 100 / 256);
    hal_display_print(0,2,HAL_DISPLAY_FONT_LARGE,buf);

    snprintf(buf, sizeof(buf), "%4d", rx * 100 / 256);
    hal_display_print(8,0,HAL_DISPLAY_FONT_LARGE,buf);

    snprintf(buf, sizeof(buf), "%4d", ry * 100 / 256);
    hal_display_print(8,2,HAL_DISPLAY_FONT_LARGE,buf);
  
  }
  
}