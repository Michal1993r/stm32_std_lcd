/*
 * utilities.c
 *
 *  Created on: 25 kwi 2016
 *      Author: root
 */
#include "stm32f4xx_tim.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_usart.h"
#include "utilities.h"

uint16_t PrescalerValue = 8400;

void PWM_TIM4_init(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOD clock enable */
  if (channel1 == ENABLE || channel2 == ENABLE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  if (channel3 == ENABLE || channel4 == ENABLE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* GPIOD Configuration */
  if (channel1 == ENABLE){
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);
  }

  if (channel2 == ENABLE){
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);
  }

  if (channel3 == ENABLE){
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
  }

  if (channel4 == ENABLE){
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
  }

  /* Connect TIM4 pins to AF2 */
  if (channel1 == ENABLE) GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);		// Channel 1
  if (channel2 == ENABLE) GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);		// Channel 2
  if (channel3 == ENABLE) GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);		// Channel 3
  if (channel4 == ENABLE) GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);		// Channel 4

  /* Compute the prescaler value */
  //PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 255;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  if (channel1 == ENABLE){
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
  }

  if (channel2 == ENABLE){
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
  }

  if (channel3 == ENABLE){
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  }

  if (channel4 == ENABLE){
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
  }

  TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

void uint16tostr(char buf[], uint32_t d, uint8_t base)
{
    uint32_t div = 1;
    uint8_t index = 0;
    while (d/div >= base) div *= base;
    while (div != 0)
    {
            uint32_t num = d/div;
            d = d%div;
            div /= base;
            if (num > 9)
                    buf[index] = (num-10) + 65;
            else
                    buf[index] = num + 48;
            index++;
    }
    buf[index] = 0;	// end of string
}

void ADC_init(){

	GPIO_InitTypeDef gpio;
	ADC_InitTypeDef adc;
	ADC_CommonInitTypeDef adcC;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AN;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpio);

	ADC_StructInit(&adc);
	adc.ADC_ContinuousConvMode = ENABLE;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_ExternalTrigConv = DISABLE;
	adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc.ADC_NbrOfConversion = 1;
	adc.ADC_Resolution = ADC_Resolution_8b;
	adc.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &adc);

	ADC_CommonStructInit(&adcC);
	adcC.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;

	adcC.ADC_Mode = ADC_Mode_Independent;
	adcC.ADC_Prescaler = ADC_Prescaler_Div4;
	adcC.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&adcC);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_480Cycles);

	ADC_Cmd(ADC1, ENABLE);

}

uint16_t ADC_Read(){

	ADC_SoftwareStartConv(ADC1);
	return ADC_GetConversionValue(ADC1);

}


void Init_SysTick(void)
{
  RCC_ClocksTypeDef RCC_Clocks;

  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000000);
}

void Delay_ms(__IO uint32_t nTime)
{
  nTime = nTime*1000;
  Delay_us(nTime);
}

void Delay_us(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)	TimingDelay--;
}

void UART_init(){

	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpio);

	USART_InitTypeDef uart;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	USART_StructInit(&uart);
	uart.USART_BaudRate = baudrate;
	uart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4, &uart);

	USART_Cmd(UART4, ENABLE);

}
void UART_Send(char a){

	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);

	UART4->DR = a;

}
void UART_Puts(const char* s){

	while(*s) UART_Send(*s++);

}
