/*
 * jk_stm32f4_tim.c
 *
 *  Created on: 2 gru 2015
 *      Author: Jakub Ko³odziejczak
 *
 *  Based on:	http://stm32f4-discovery.com/2014/05/stm32f4-stm32f429-discovery-pwm-tutorial/
 *  			http://visualgdb.com/tutorials/arm/stm32/timers/
 *
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */

#include "jk_stm32f4_tim.h"

void JK_TIMER_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;

    /* Enable clock for TIM5 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
/*
    timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)
*/
    TIM_BaseStruct.TIM_Prescaler = 1-1;
    /* Count up */
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
/*
    Set timer period when it resets:
    TIM_Period = timer_tick_frequency / desired_frequency - 1

    If you get TIM_Period larger than max timer value (in our case 2^32),
    you have to choose larger prescaler and slow down timer tick frequency
*/
    TIM_BaseStruct.TIM_Period = 37500-1;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    /* Initialize TIM5 */
    TIM_TimeBaseInit(TIM5, &TIM_BaseStruct);
    /* Start count on TIM5 */
    TIM_Cmd(TIM5, ENABLE);
    /* Enable update */
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

void JK_IRQ_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;

    //TIM5 - sampler
    NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    //TIM10 - button IRQ
    /* Tell system that you will use PA0 for EXTI_Line0 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    /* PA0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);
    /* Add IRQ vector to NVIC */
    /* PA0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}
