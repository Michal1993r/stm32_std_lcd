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

uint16_t 	temp_i,
			temp_o,
			percent,
			temp_d = 24,
 			Kp = 1,
			Ki = 1,
			Kd = 1;
int16_t		P = 0,
			I = 0,
			D = 0,
			pwm = 0,
			e = 0,
			e_old = 0,
			gain = 1;

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

	// Variables

	char buff[32];
	uint16_t temp;

	// Create menu
	menu.Text_color = LCD_WHITE;
	menu.Tooltip_color = LCD_RED;
	menu.frame_color = LCD_RED;
	menu.Progress_bar_bckground_color = LCD_BLACK;

	TIM4->CCR1 = CCR1_Val;

	LCD_Clear_Screen(LCD_BLUE);
	Menu_create(&menu);

	temp_i = 0;
	temp_o = 0;

  while(1)
  {
	  // Temperature measurement
	  DS18B20_onetemp(sensors, 0, 9);
	  DS18B20_onetemp(sensors, 1, 9);
	  temp_o = sensors[0].temp;
	  temp_i = sensors[1].temp;
	  MENU_TempIn(&menu, itoa(temp_i, buff, 10));
	  Menu_TempOut(&menu, itoa(temp_o, buff, 10));

	  e_old = e;
	  e = temp_d - temp_i;
	  P = e*Kp;
	  I = Ki*(e_old + e);
	  D = Kd*(e_old - e);
	  pwm = (P + I + D)*gain;

	  if (pwm < 0) pwm = -pwm;
	  if (pwm > 255) pwm = 255;

	  TIM4->CCR2 = pwm;
	  UART_Puts(itoa(pwm, buff, 10));


	  // uchyb ujemny lub dodatni
	  if (e > 0) heat_up();
	  else if (e < 0 ) cool_down();

	  // sterowanie wiatraczkami
	  if (ModeFlag == AUTO){

		  Menu_FanSpeed(&menu, 50);
		  if (e > 0) heater_fan();
		  else if (e < 0 ) cooler_fan();

	  } else if (ModeFlag == MANUAL){

		 temp = (ADC_Read()*100)/255;
		 percent = (uint16_t)temp;
		 //if (temp > 180) temp = 180;
		 Menu_FanSpeed(&menu, percent);
		 if (e > 0) {

			 TIM4->CCR3 = temp;
			 TIM4->CCR4 = 0;

		 }
		 else if (e < 0){

			 TIM4->CCR4 = temp;
			 TIM4->CCR3 = 0;

		 }

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

	TIM4->CCR3 = 255;
	TIM4->CCR4 = 0;

}

void cooler_fan(){

	TIM4->CCR3 = 255;
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
