#ifndef __HAL_H_
#define __HAL_H_

#include <Arduino.h>

#define HAL_DISPLAY_WIDTH             128
#define HAL_DISPLAY_HEIGHT            64

#ifdef __cplusplus
extern "C" {
#endif

// ────────────────────────────────────────────────
//                  COMMON
// ────────────────────────────────────────────────

void  hal_setup();
void  hal_loop();

// ────────────────────────────────────────────────
//                  JOYSTICK
// ────────────────────────────────────────────────

void hal_joystick_get(int16_t& lx, int16_t& ly,int16_t& rx, int16_t& ry, bool& lb, bool& rb);

// ────────────────────────────────────────────────
//                  KEYBOARD
// ────────────────────────────────────────────────

void hal_keyboard_get(bool& lb, bool& rb, bool& tb, bool& bb);

// ────────────────────────────────────────────────
//                  DISPLAY
// ────────────────────────────────────────────────

enum HAL_DISPLAY_FONT
{
    HAL_DISPLAY_FONT_SMALL,
    HAL_DISPLAY_FONT_MEDIUM,
    HAL_DISPLAY_FONT_LARGE,
};

void hal_display_clear(void);
void hal_display_print(int x, int y, HAL_DISPLAY_FONT fontSize, const char* str);
void hal_display_printf(int x, int y, HAL_DISPLAY_FONT fontSize, const char* format, ...);
void hal_display_refresh(void);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_H_ */