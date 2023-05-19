#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>

#include "REC_3DP_Config.h"

#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

namespace Display {
    void Init();
    void update();
    void print_menu();

    extern long currMillis_Display;
    extern long prevMillis_Display;
    extern LiquidCrystal_I2C lcd;

    extern int current_menu_item;
}  // namespace Display

#endif
