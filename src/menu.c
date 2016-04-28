/*
 * menu.c
 *
 *  Created on: 22 kwi 2016
 *      Author: root
 */
#include "menu.h"

void Menu_create(Menu* menu){


	LCD_Set_Font(&Font16x24);
		  // BLOK TEMP OUT
		  LCD_Display_String(5, 309, "TEMP OUT", menu->frame_color);
		  LCD_Draw_Full_Rect(27, 309, 47, 180, menu->frame_color );
		  LCD_Draw_Circle(32, 208, 3, menu->Text_color);
		  LCD_Display_String(28, 203, "C", menu->Text_color);
		  LCD_Display_String(28, 300, "00.00", menu->Text_color);
		  //BLOK TEMP IN
		  LCD_Display_String(52, 309, "TEMP IN", menu->Tooltip_color);
		  LCD_Draw_Full_Rect(74, 309, 94, 180, menu->frame_color);
		  LCD_Draw_Circle(79, 208, 3, menu->Text_color);
		  LCD_Display_String(75, 203, "C", menu->Text_color);
		  LCD_Display_String(75, 300, "00.00", menu->Text_color);
		  //BLOK FAN
		  LCD_Display_String(5, 108," FAN  ", menu->Tooltip_color);
		  LCD_Display_String(28, 108, "SPEED", menu->Tooltip_color);
		  LCD_Draw_ProgressBar(51, 128, 71, 10, 0, menu->Progress_bar_bckground_color);
		  LCD_Draw_Full_Rect(74, 116, 94, 15, menu->frame_color);
		  LCD_Display_String(75, 113, "MANUAL", menu->Text_color);
		  //BLOK PROGRAM TEMP
		  LCD_Display_String(130, 254, "DESIRED TEMP", menu->Tooltip_color);
		  LCD_Draw_Full_Rect(152, 249, 171, 69, menu->frame_color);
		  LCD_Draw_Circle(157, 128, 3, menu->Text_color);
		  LCD_Display_String(152, 123, "C", menu->Text_color);
		  LCD_Display_String(152, 219, "24.0", menu->Text_color);
		  // PRZYCISKI
		  // -
		  LCD_Draw_Full_Rect(181, 289, 231, 169, LCD_GREY);
		  LCD_Draw_Line(206, 259, 206, 199, menu->frame_color); // -
		  // +
		  LCD_Draw_Full_Rect(181, 149, 231, 29, LCD_GREY);
		  LCD_Draw_Line(206, 119, 206, 59, menu->frame_color); // -
		  LCD_Draw_Line(191, 89, 221, 89, menu->frame_color); // |

}

void Menu_FanSpeed(Menu* m, uint8_t percentage){

	LCD_Draw_ProgressBar(51, 128, 71, 10, percentage, m->Progress_bar_bckground_color);

}

void MENU_TempIn(Menu* m, char* c){

	LCD_Draw_Full_Rect(74, 309, 94, 180, m->frame_color);
	LCD_Draw_Circle(79, 208, 3, m->Text_color);
	LCD_Display_String(75, 300, c, m->Text_color);
	LCD_Display_String(75, 203, "C", m->Text_color);

}

void Menu_TempOut(Menu* menu, char* c){


	LCD_Draw_Circle(32, 208, 3, menu->Text_color);
	LCD_Display_String(28, 300, c, menu->Text_color);
	LCD_Display_String(28, 203, "C", menu->Text_color);

}

void Menu_Desiredtemp(Menu* menu, char * c){

	LCD_Draw_Full_Rect(152, 249, 171, 69, menu->frame_color);
	LCD_Draw_Circle(157, 128, 3, menu->Text_color);
	LCD_Display_String(152, 219, c, menu->Text_color);
	LCD_Display_String(152, 123, "C", menu->Text_color);

}

void Menu_FanControl(Menu* menu, int ManualOrAuto){

	LCD_Draw_Full_Rect(74, 116, 94, 15, menu->frame_color);
	if (ManualOrAuto == MANUAL) LCD_Display_String(75, 113, "MANUAL", menu->Text_color);
	else if (ManualOrAuto == AUTO) LCD_Display_String(75, 113, " AUTO ", menu->Text_color);

}

uint8_t Menu_PlusPressed(){

	Read_Ads7846();
	bx = Pen_Point.X;
	by = Pen_Point.Y;
	if (((bx >= 181) && (bx <= 231)) && ((by >= 59) && (by <= 119))){
		//Delay_ms(100);
		Pen_Point.X = 0;
		Pen_Point.Y = 0;
		return 1;
	}
	else return 0;

}

uint8_t Menu_MinusPressed(){

	Read_Ads7846();
	bx = Pen_Point.X;
	by = Pen_Point.Y;
	if (((bx >= 181) && (bx <= 231)) && ((by >= 169) && (by <= 289))){
		//Delay_ms(100);
		Pen_Point.X = 0;
		Pen_Point.Y = 0;
		return 1;
	}
	else return 0;

}

