/*
 * utilities.h
 *
 *  Created on: 25 kwi 2016
 *      Author: root
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

void PWM_TIM4_init(void);
void uint16tostr(char buf[], uint32_t d, uint8_t base);

void ADC_init();
int ADC_Read(); // TODO


#endif /* UTILITIES_H_ */
