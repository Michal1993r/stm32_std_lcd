/**
 *
 */
/* Includes *******************************************************************/
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "ts.h"
#include "LCD_STM32F4.h"
#include "menu.h"
#include "utilities.h"
#include "img/img00.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
Menu menu;
uint16_t CCR1_Val = 2500;	//LCD Brightness
extern uint8_t Touched;		//Touch panel flag

int main (void)
{
  /* Initialization */

	SystemInit();

	LCD_Init();
	PWM_TIM4_init();
	touch_init();

	int8_t temp_i, temp_o, temp_d = 24;

	char buff[10];

	menu.Text_color = LCD_WHITE;
	menu.Tooltip_color = LCD_RED;
	menu.frame_color = LCD_RED;
	menu.Progress_bar_bckground_color = LCD_BLACK;

  TIM4->CCR1 = CCR1_Val;

  LCD_Clear_Screen(LCD_BLUE);
  Menu_create(&menu);

  //DMA_InitTypeDef dma;

  while(1)
  {

	  if(Touched){

		  if (Menu_PlusPressed()){

			  temp_d += 1;
			  Menu_Desiredtemp(&menu, itoa(temp_d, buff, 10));
			  Touched = 0;

		  }
		  else if (Menu_MinusPressed()){

			  temp_d -= 1;
			  Menu_Desiredtemp(&menu, itoa(temp_d, buff, 10));
			  Touched = 0;

		  }
	  }


  }

  return 0;
}
