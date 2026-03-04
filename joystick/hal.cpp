#include "hal.h"
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <GyverJoy.h>


#define SERIAL_BAUD  115200

#define HAL_DISPLAY_PIN_CLOCK   A5
#define HAL_DISPLAY_PIN_DATA    A4
#define HAL_DISPLAY_PIN_RESET   U8X8_PIN_NONE

#define HAL_JOYSTICK_LEFT_X A3
#define HAL_JOYSTICK_LEFT_Y A2
#define HAL_JOYSTICK_LEFT_SW 2

#define HAL_JOYSTICK_RIGHT_X A1
#define HAL_JOYSTICK_RIGHT_Y A0
#define HAL_JOYSTICK_RIGHT_SW 3

#define LOG(str) Serial.print("#=> "); Serial.println(str);

// ────────────────────────────────────────────────
//                  JOYSTICK
// ────────────────────────────────────────────────

GyverJoy joyLX(HAL_JOYSTICK_LEFT_X);
GyverJoy joyLY(HAL_JOYSTICK_LEFT_Y);
GyverJoy joyRX(HAL_JOYSTICK_RIGHT_X);
GyverJoy joyRY(HAL_JOYSTICK_RIGHT_Y);

void hal_joystick_setup(void)
{
    joyLX.invert(true);
    joyLX.calibrate();
    joyLX.deadzone(40);         
    joyLX.exponent(GJ_SQUARE  );         

    joyLY.invert(true);
    joyLY.calibrate();
    joyLY.deadzone(40);         
    joyLY.exponent(GJ_SQUARE  );         

    joyRX.invert(true);
    joyRX.calibrate();
    joyRX.deadzone(40);         
    joyRX.exponent(GJ_SQUARE );         

    joyRY.invert(true);
    joyRY.calibrate();
    joyRY.deadzone(40);         
    joyRY.exponent(GJ_SQUARE );         
}

void hal_joystick_loop(void)
{
  joyLX.tick();
  joyLY.tick();
  joyRX.tick();
  joyRY.tick();
}

void hal_joystick_get(int16_t& lx, int16_t& ly,int16_t& rx, int16_t& ry)
{
    lx = joyLX.value();
    ly = joyLY.value();
    rx = joyRX.value();
    ry = joyRY.value();
}


// ────────────────────────────────────────────────
//                  DISPLAY
// ────────────────────────────────────────────────

U8X8_SH1106_128X64_NONAME_HW_I2C u8g(HAL_DISPLAY_PIN_CLOCK, HAL_DISPLAY_PIN_DATA, HAL_DISPLAY_PIN_RESET);

void hal_display_setup()
{
    u8g.begin();
    u8g.setPowerSave(0);
}

void hal_display_loop()
{
    // do nothing
}

void hal_display_clear(void)
{
    u8g.clearDisplay();
}

void hal_display_printf(int x, int y, HAL_DISPLAY_FONT fontSize, const char* format, ...)
{
    char buffer[HAL_DISPLAY_WIDTH];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    hal_display_print(x,y,fontSize,buffer);
}

void hal_display_print(int x, int y, HAL_DISPLAY_FONT fontSize, const char* str)
{
    switch (fontSize)
    {
        case HAL_DISPLAY_FONT_SMALL:
            u8g.setFont(u8x8_font_chroma48medium8_r);
            break;
        case HAL_DISPLAY_FONT_MEDIUM:
            u8g.setFont(u8x8_font_chroma48medium8_r);
            break;
        case HAL_DISPLAY_FONT_LARGE:
            u8g.setFont(u8x8_font_px437wyse700a_2x2_f);
            break;
        default:
            u8g.setFont(u8x8_font_chroma48medium8_r);  // дефолт
            break;
    }
    u8g.drawString(x,y,str);
}

void hal_display_refresh(void)
{
    u8g.refreshDisplay();
}


// ────────────────────────────────────────────────
//                  COMMON
// ────────────────────────────────────────────────

void hal_setup()
{
    Serial.begin(SERIAL_BAUD);
    delay(100);
    Serial.println("#https://github.com/asvol/asv-robot-arm");
    hal_display_setup();
    hal_joystick_setup();
}

void hal_loop()
{
    hal_display_loop();
    hal_joystick_loop();
}