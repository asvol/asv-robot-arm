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
#define HAL_JOYSTICK_LEFT_SW D3

#define HAL_JOYSTICK_RIGHT_X A1
#define HAL_JOYSTICK_RIGHT_Y A0
#define HAL_JOYSTICK_RIGHT_SW D2

#define LOG(str) Serial.print("#=> "); Serial.println(str);

// Состояния
enum HAL_BUTTON_STATE 
{
    HAL_BUTTON_STATE_IDLE,               // кнопка отпущена, ждём нажатия
    HAL_BUTTON_STATE_DEBOUNCE,           // защита от дребезга
    HAL_BUTTON_STATE_PRESSED,            // кнопка нажата, считаем время
    HAL_BUTTON_STATE_WAITING_RELEASE,    // уже сработало действие, ждём отпускания
};

// ────────────────────────────────────────────────
//                  JOYSTICK
// ────────────────────────────────────────────────

GyverJoy joyLX(HAL_JOYSTICK_LEFT_X);
GyverJoy joyLY(HAL_JOYSTICK_LEFT_Y);
GyverJoy joyRX(HAL_JOYSTICK_RIGHT_X);
GyverJoy joyRY(HAL_JOYSTICK_RIGHT_Y);

HAL_BUTTON_STATE lb = HAL_BUTTON_STATE_IDLE;
HAL_BUTTON_STATE rb = HAL_BUTTON_STATE_IDLE;

void hal_joystick_setup(void)
{
    //GJ_LINEAR (умолч.), GJ_SQUARE и GJ_CUBIC или цифрами 0, 1 и 2
    joyLX.invert(true);
    joyLX.calibrate();
    joyLX.deadzone(40);         
    joyLX.exponent(GJ_LINEAR  );         

    joyLY.invert(true);
    joyLY.calibrate();
    joyLY.deadzone(40);         
    joyLY.exponent(GJ_LINEAR  );         

    joyRX.invert(true);
    joyRX.calibrate();
    joyRX.deadzone(40);         
    joyRX.exponent(GJ_LINEAR );         

    joyRY.invert(true);
    joyRY.calibrate();
    joyRY.deadzone(40);         
    joyRY.exponent(GJ_LINEAR );         
}

void hal_joystick_loop(void)
{
  joyLX.tick();
  joyLY.tick();
  joyRX.tick();
  joyRY.tick();
}

void hal_joystick_get(int16_t& lx, int16_t& ly,int16_t& rx, int16_t& ry, bool& lb, bool& rb)
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