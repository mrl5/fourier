/*
 * definitions.h
 *
 *  Created on: 3 gru 2015
 *      Author: Jakub Ko³odziejczak
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* FFT settings */
#define SAMPLES					(512)
#define FFT_SIZE				(SAMPLES / 2)
#define FFT_BAR_MAX_HEIGHT		155           //in pixels

/* TIMER settings */
//#define BUTTON_DELAY (9) // 50 ms
/*
#define TIM_PERIOD				(7500)		256->7500
											512->3750
											1024->1875 */

/* ADC settings */
#define uADC ADC1
#define uADC_CHANNEL ADC_Channel_4

/* LCD settings */
#define MAIN_BACKGROUND			ILI9341_COLOR_WHITE
#define TITLE_COLOR				ILI9341_COLOR_BLACK
#define TITLE_BACKGROUND		ILI9341_COLOR_YELLOW
#define AUTHOR_COLOR			0x012C
#define AUTHOR_BACKGROUND		MAIN_BACKGROUND
#define FFT_COLOR				ILI9341_COLOR_WHITE
#define FFT_BACKGROUND			ILI9341_COLOR_BLACK
#define M_COLOR					ILI9341_COLOR_BLACK
#define M_BACKGROUND			MAIN_BACKGROUND
#define L1_COLOR				ILI9341_COLOR_YELLOW
#define L2_COLOR				ILI9341_COLOR_GREEN
#define L3_COLOR				ILI9341_COLOR_CYAN

#endif
