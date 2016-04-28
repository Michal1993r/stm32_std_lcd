/*
 * menu.h
 *
 *  Created on: 22 kwi 2016
 *      Author: root
 */

#ifndef MENU_H_
#define MENU_H_

#include "LCD_STM32F4.h"
#include "ts.h"

#define MANUAL	0
#define AUTO	1

uint16_t bx, by;

typedef struct MENU{
	uint16_t Tooltip_color;
	uint16_t frame_color;
	uint16_t Text_color;
	uint16_t Progress_bar_bckground_color;
}Menu;

extern void Menu_create(Menu* m);
extern void Menu_FanSpeed(Menu* m, uint8_t percentage);
extern void MENU_TempIn(Menu* m, char* c);
extern void Menu_TempOut(Menu* menu, char* c);
extern void Menu_Desiredtemp(Menu* menu, char *);
extern void Menu_FanControl(Menu* menu, int ManualOrAuto);
extern uint8_t Menu_PlusPressed();
extern uint8_t Menu_MinusPressed();

#endif /* MENU_H_ */
