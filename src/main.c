/**
 *
 */
/* Includes *******************************************************************/
#include <stdlib.h>
#include "pwm.h"
#include "ts.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "LCD_STM32F4.h"
#include "functions.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 500;

int main (void)
{
  /* Initialization */

	SystemInit();

	LCD_Init();
	pwm_init();
	touch_init();

  TIM4->CCR1 = CCR1_Val;

  LCD_Clear_Screen(LCD_BLUE);

  while(1)
  {
	  LCD_Set_Font(&Font16x24);
	  // BLOK TEMP OUT
	  LCD_Display_String(5, 309, "TEMP OUT", LCD_RED);
	  LCD_Draw_Rect(27, 309, 47, 180, LCD_RED );
	  LCD_Draw_Circle(32, 208, 3, LCD_WHITE);
	  LCD_Display_String(28, 300, "19.02 C", LCD_WHITE);
	  //BLOK TEMP IN
	  LCD_Display_String(52, 309, "TEMP IN", LCD_RED);
	  LCD_Draw_Rect(74, 309, 94, 180, LCD_RED);
	  LCD_Draw_Circle(79, 208, 3, LCD_WHITE);
	  LCD_Display_String(75, 300, "22.54 C", LCD_WHITE);
	  //BLOK FAN
	  LCD_Display_String(5, 108," FAN  ", LCD_RED);
	  LCD_Display_String(28, 108, "SPEED", LCD_RED);
	  LCD_Draw_ProgressBar(51, 128, 71, 10, 0, LCD_BLACK);
	  LCD_Draw_Rect(74, 116, 94, 15, LCD_RED);
	  LCD_Display_String(75, 113, "MANUAL", LCD_WHITE);
	  //BLOK PROGRAM TEMP
	  LCD_Display_String(130, 254, "DESIRED TEMP", LCD_RED);
	  LCD_Draw_Rect(152, 249, 171, 69, LCD_RED);
	  LCD_Display_String(152, 249, "  20.5  C", LCD_WHITE);
	  // PRZYCISKI
	  LCD_Draw_Rect(181, 289, 231, 169, LCD_RED);
	  LCD_Draw_Rect(181, 149, 231, 29, LCD_RED);

	  for ( uint16_t i = 0;  i <= 100; i = i+5){
	 	  LCD_Draw_ProgressBar(51, 128, 71, 10, i, LCD_BLACK);
	 	  Delay_ms(250);
	 	  }

  }

  return 0;
}
