/*
 * DS18B20.h
 *
 *  Created on: 8 cze 2016
 *      Author: root
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <utilities.h>
#include <stdlib.h>

// PE5

#define SENSNUMBER		2

typedef struct
{
	char	addr[8];
	int		temp;

}sDS18B20;

char buff[16];

GPIO_InitTypeDef gpio_ds;

uint8_t DS18B20_temp(sDS18B20[]);
void DS18B20_onetemp(sDS18B20[], uint8_t, uint8_t);
void DS18B20_init(sDS18B20[]);
void DS18B20_readaddress(void);


#endif /* DS18B20_H_ */
