/*
 * utilities.h
 *
 *  Created on: 25 kwi 2016
 *      Author: root
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_
#include "stm32f4xx.h"

#define baudrate 115200

static __IO uint32_t TimingDelay;

void ADC_init();
uint16_t ADC_Read();

void PWM_TIM4_init(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4);
void uint16tostr(char buf[], uint32_t d, uint8_t base);

void Init_SysTick(void);
void Delay_ms(__IO uint32_t nTime);
void Delay_us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void UART_init();
void UART_Send(char a);
void UART_Puts(const char* a);


#endif /* UTILITIES_H_ */
