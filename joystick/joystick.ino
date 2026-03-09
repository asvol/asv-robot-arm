#include "hal.h"

unsigned long previousMillis = 0;     // запоминаем время последнего обновления
const unsigned long interval = 100;   // 100 мс = 0.1 секунды

void setup()
{
  hal_setup();
  pinMode(A6, INPUT);
}

void loop()
{
  unsigned long currentMillis = millis();
  hal_loop();
  int16_t lx = 0;
  int16_t ly = 0;
  int16_t rx = 0;
  int16_t ry = 0;
  bool lb = false;
  bool rb = false;
  hal_joystick_get(lx,ly,rx,ry, lb, rb);
 
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
  
    if (lb)
    {
      hal_display_print(0,4,HAL_DISPLAY_FONT_LARGE,"++++");
    }
    else
    {
      hal_display_print(0,4,HAL_DISPLAY_FONT_LARGE,"    ");
    }
      
    if (rb)
    {
      hal_display_print(8,4,HAL_DISPLAY_FONT_LARGE,"++++");
    }
    else
    {
      hal_display_print(8,4,HAL_DISPLAY_FONT_LARGE,"    ");
    }

    int val = analogRead(A6);
    snprintf(buf, sizeof(buf), "%-4d", val);
    hal_display_print(0,6,HAL_DISPLAY_FONT_LARGE,buf);



    Serial.print("RX="); Serial.print(rx);
    Serial.print("RY="); Serial.print(ry);
    Serial.print("LX="); Serial.print(lx);
    Serial.print("LY="); Serial.print(ly);
    Serial.print("LB="); Serial.print(lb);
    Serial.print("RB="); Serial.print(rb);
    Serial.print("REC="); Serial.print(0);
    Serial.println();
  }
  
}
