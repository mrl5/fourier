/*
 * jk_stm32f4_mux.h
 *
 *  Created on: 19 gru 2015
 *      Author: Jakub Ko³odziejczak
 */

#ifndef JK_STM32F4_MUX_H_
#define JK_STM32F4_MUX_H_

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "definitions.h"

void JK_SWITCH_Init(void);
void JK_MUX_Init(void);

#endif /* JK_STM32F4_MUX_H_ */
