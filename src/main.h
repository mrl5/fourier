/*
 * main.h
 *
 *  Created on: 3 gru 2015
 *      Author: Jakub Ko³odziejczak
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "arm_math.h"
#include <stdio.h>
#include <math.h>

#include "defines.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_fft.h"
#include "tm_stm32f4_ili9341.h"

#include "tm_stm32f4_adc.h"
#include "jk_stm32f4_tim.h"
#include "jk_stm32f4_mux.h"

void TIM5_IRQHandler();		//interrupts from TIM5 - sampling
void EXTI0_IRQHandler();	//interrupts from button - signal switching
void SignalSwitch();
void DrawPhase(uint8_t i);
void DrawBar(uint16_t bottomX, uint16_t bottomY, uint16_t maxHeight, uint16_t maxValue, float32_t value, uint16_t foreground, uint16_t background);
void MainInit();

#endif /* MAIN_H_ */
