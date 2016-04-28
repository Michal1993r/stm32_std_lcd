/*
 * dcmi.c
 *
 *  Created on: 22 kwi 2016
 *      Author: root
 */
#include "stm32f4xx.h"
#include "stm32f4xx_dcmi.h"

void DCMI_init(){

	GPIO_InitTypeDef gpio;
	DCMI_InitTypeDef dcmi;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
			| RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);	//HSync
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);	//Pixlclock
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);	//D5
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);	//D6
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);	//D7
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);	//D4
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_DCMI);	//D3
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_DCMI);	//D2
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);	//D1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);	//D0

	gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &gpio);

	///////////////////////////////	DCMI CONFIG	///////////////////////////////////

	dcmi.DCMI_CaptureMode = DCMI_CaptureMode_SnapShot;
	dcmi.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	dcmi.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
	dcmi.DCMI_HSPolarity = DCMI_HSPolarity_High;
	dcmi.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
	dcmi.DCMI_SynchroMode = DCMI_SynchroMode_Embedded;
	dcmi.DCMI_VSPolarity = DCMI_VSPolarity_High;



}

