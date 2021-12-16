/*
 * jk_stm32f4_mux.c
 *
 *  Created on: 19 gru 2015
 *      Author: Jakub Ko³odziejczak
 */


#include "jk_stm32f4_mux.h"

void JK_SWITCH_Init()
{
    GPIO_InitTypeDef GPIO_InitDef;
    //Enable clock for GPOIA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //Pin 0
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
    //Mode output
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
    //Output type push-pull
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    //With pull down resistor
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
    //50MHz pin speed
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
    //Initialize pin on GPIOA port
    GPIO_Init(GPIOA, &GPIO_InitDef);
}

void JK_MUX_Init()
{
    GPIO_InitTypeDef GPIO_InitDef;
    //Enable clock for GPOIF
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    //Pin 11
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_11;
    //Mode output
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    //Output type push-pull
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    //With pull DOWN resistor
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
    //2 MHz pin speed
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
    //Initialize pin on GPIOF port
    GPIO_Init(GPIOF, &GPIO_InitDef);
    GPIO_SetBits(GPIOF, GPIO_Pin_11);

    //Pin 13, 15
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    //Mode output
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    //Output type push-pull
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    //With pull down resistor
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
    //2 MHz pin speed
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
    //Initialize pin on GPIOF port
    GPIO_Init(GPIOF, &GPIO_InitDef);
    GPIO_ResetBits(GPIOF, GPIO_Pin_13 | GPIO_Pin_15);
}
