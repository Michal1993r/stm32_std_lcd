/**
 *
 */
/* Includes *******************************************************************/
#include "main.h"
#include "fota.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 500;

int main (void)
{
  /* Initialization */

	SystemInit();
	Init_SysTick();
	Init_GPIO();
	Init_FSMC();
	Init_LCD();
	pwm_init();
	touch_init();

  /*!< At this stage the microcontroller clock setting is already configured,
         this is done through SystemInit() function which is called from startup
         file (startup_stm32f4xx.s) before to branch to application main.
         To reconfigure the default setting of SystemInit() function, refer to
         system_stm32f4xx.c file
       */

  TIM4->CCR1 = CCR1_Val;

  Clear_Screen(LCD_BLUE);

  Draw_Image(0,0,240,320,&fota);

  while(1)
  {

	  read_once();
	  Delay_ms(250);

  }

  return 0;
}
