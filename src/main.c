/**
 *
 */
/* Includes *******************************************************************/
#include <stdlib.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "ts.h"
#include "LCD_STM32F4.h"
#include "menu.h"
#include "utilities.h"
#include "DS18B20.h"

void peltier_init();
void heat_up();
void cool_down();
void heater_fan();
void cooler_fan();

Menu menu;
uint16_t CCR1_Val = 255;	//LCD Brightness 0 - 255
sDS18B20 sensors[SENSNUMBER];

uint16_t temp_i, temp_o, percent;
int8_t temp_d = 24;

uint8_t ModeFlag = AUTO;

int main (void)
{
  /* Initialization */

	SystemInit();
	Init_SysTick();

	LCD_Init();
	PWM_TIM4_init(ENABLE, ENABLE, ENABLE, ENABLE);
	touch_init();
	ADC_init();
	peltier_init();
	DS18B20_init(sensors);
	UART_init();

	char buff[32];
	uint16_t temp;

	menu.Text_color = LCD_WHITE;
	menu.Tooltip_color = LCD_RED;
	menu.frame_color = LCD_RED;
	menu.Progress_bar_bckground_color = LCD_BLACK;

	TIM4->CCR1 = CCR1_Val;


	LCD_Clear_Screen(LCD_BLUE);
	Menu_create(&menu);

  while(1)
  {
	  DS18B20_onetemp(sensors, 0, 9);
	  DS18B20_onetemp(sensors, 1, 9);
	  temp_i = sensors[0].temp;
	  temp_o = sensors[1].temp;
	  MENU_TempIn(&menu, itoa(temp_i, buff, 10));
	  Menu_TempOut(&menu, itoa(temp_o, buff, 10));

	  //TODO REGULATOR

	  if (ModeFlag == AUTO){

		  Menu_FanSpeed(&menu, 50);

	  } else if (ModeFlag == MANUAL){

		 temp = (ADC_Read()*100)/255;
		 percent = (uint16_t)temp;
		 Menu_FanSpeed(&menu, percent);

	  }
  }


  return 0;
}

void peltier_init(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef gpio;

	gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC, &gpio);

}

void heat_up(){

	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);

}

void cool_down(){

	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);

}

void heater_fan(){

	TIM4->CCR3 = 125;
	TIM4->CCR4 = 0;

}

void cooler_fan(){

	TIM4->CCR3 = 125;
	TIM4->CCR4 = 0;

}

void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {

	  if (Menu_PlusPressed()){

		  temp_d += 1;
		  Menu_Desiredtemp(&menu, itoa(temp_d, buff, 10));

	  }
	  else if (Menu_MinusPressed()){

		  temp_d -= 1;
		  Menu_Desiredtemp(&menu, itoa(temp_d, buff, 10));

	  }
	  else if (Menu_AutoPressed()){

		  if (ModeFlag == AUTO){

			  Menu_FanControl(&menu, MANUAL);
			  ModeFlag = MANUAL;

		  } else if (ModeFlag == MANUAL){

			  Menu_FanControl(&menu, AUTO);
			  ModeFlag = AUTO;
		  }

	  }

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
}
