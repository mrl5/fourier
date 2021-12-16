/*
 * jk_stm32f4_tim.h
 *
 *  Created on: 2 gru 2015
 *      Author: Jakub Ko³odziejczak
 */

#ifndef JK_STM32F4_TIM_H_
#define JK_STM32F4_TIM_H_

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "definitions.h"

void JK_TIMER_Init(void);
void JK_IRQ_Init(void);

#endif /* JK_STM32F4_TIM_H_ */
