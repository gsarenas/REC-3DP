#ifndef DISPLAY_H
#define DISPLAY_H

#include "REC_3DP_Config.h"
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

void Init_Display();
void update_display();
void print_menu();

extern long currMillis_Display;
extern long prevMillis_Display;
extern LiquidCrystal_I2C lcd;

//extern int motorSpeed;
//extern int targetTemp;
extern int current_menu_item;

#endif
