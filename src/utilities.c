/*
 * utilities.c
 *
 *  Created on: 25 kwi 2016
 *      Author: root
 */
#include "stm32f4xx_tim.h"
#include "stm32f4xx_adc.h"
#include "utilities.h"

uint16_t PrescalerValue = 0;

void PWM_TIM4_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* GPIOD Configuration: TIM4 CH1 (PD12) and TIM4 CH2 (PD13) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; // | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Connect TIM4 pins to AF2 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

//  // Pin 13 ... LED
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // | GPIO_Pin_13 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
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

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

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

	gpio.GPIO_Pin = GPIO_Pin_0;
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

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);

	ADC_Cmd(ADC1, ENABLE);

}

uint16_t ADC_Read(){

	ADC_SoftwareStartConv(ADC1);
	return ADC_GetConversionValue(ADC1);

}
