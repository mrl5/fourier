/*
 * FOURIER
 *
 *  Last change: 15 January 2016
 *      Author: Jakub Ko³odziejczak
 *      Based on: Tilen Majerle libs http://stm32f4-discovery.com
 *      License: MIT
 *
Copyright (c) 2016 Jakub Ko³odziejczak

Permission is hereby granted, free of charge,
to any person obtaining a copy of this software
and associated documentation files (the "Software"),
to deal in the Software without restriction,
including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "main.h"
#include "definitions.h" //FFT and other settings declared here
#define version					(1.00)

float32_t Input[SAMPLES];
float32_t Output[FFT_SIZE];
const float32_t res = 4096.0-1.0;
const float32_t DC_bias = 2107.0;//1954.0;//2048.0;
const float32_t current_calc = 25.0/0.625;
TM_FFT_F32_t FFT;    		/* FFT structure */
uint8_t fft_flag = 0;		//1 when input buffer is filled and FFT calculation can start
int8_t mc = 0;				//mask counter

int main(void)
{
	float32_t RMS;
	float32_t n = SAMPLES;
	const float32_t Uref = 2.9;
	char author[45];
	char m[10];					//measurement output array
	char u[2];					//unit
	const uint16_t margin = 5;  //fft bar margin
	uint16_t bar = 0;			//fft bar counter
	uint8_t a = 0;				//author U+006C->U+0142

	/* LCD objects positioning */
	uint16_t title_pos[2] = {125, 2};											//title position on LCD
	uint16_t title_frame[2][2] = { {0, ILI9341_HEIGHT}, {0, 18+title_pos[1]} };
	uint16_t author_pos[2] = {2, 229};
	uint16_t a_sign[2] = {author_pos[0]+198, author_pos[1]+5};
	uint16_t fft_pos[2][2] = { {10, 310}, {60, 225} };							//spectrum window position {x0,x1},{y0,y1}
	uint16_t m_pos[2] = { fft_pos[0][0], 40 };									//RMS measurement position

	/* Initialize program */
	MainInit();

	/* Print title and spectrum window on LCD */
	TM_ILI9341_DrawFilledRectangle(title_frame[0][0], title_frame[1][0], title_frame[0][1], title_frame[1][1], TITLE_BACKGROUND);
	TM_ILI9341_Puts(title_pos[0], title_pos[1], "FOURIER", &TM_Font_11x18, TITLE_COLOR, TITLE_BACKGROUND);

	sprintf(author,"version %1.2f    (C) Jakub Kolodziejczak, 2016",version);
	TM_ILI9341_Puts(author_pos[0], author_pos[1], author, &TM_Font_7x10, AUTHOR_COLOR, AUTHOR_BACKGROUND);
	for (a = 0; a < 3; a++)
		TM_ILI9341_DrawLine(a_sign[0]+a, a_sign[1]-a, a_sign[0]+a, a_sign[1]-a, AUTHOR_COLOR);

	TM_ILI9341_DrawFilledRectangle(fft_pos[0][0], fft_pos[1][0], fft_pos[0][1], fft_pos[1][1], FFT_BACKGROUND);

	/* Start measuring - enable MUX input */
	GPIO_ResetBits(GPIOF, GPIO_Pin_11);

	sprintf(u, "A");
	TM_ILI9341_Puts(m_pos[0]+110, m_pos[1], u, &TM_Font_11x18, M_COLOR, M_BACKGROUND);

	while (1)
	{
		if (fft_flag == 1)
		{
			TM_FFT_Process_F32(&FFT);	//calculate FFT
			fft_flag = 0;
			bar = 0;
			RMS = pow(2.0*TM_FFT_GetFromBuffer(&FFT, bar)/n,2);
			DrawBar(fft_pos[0][0]+margin + 2 * bar, fft_pos[1][1]-margin,
					FFT_BAR_MAX_HEIGHT, TM_FFT_GetMaxValue(&FFT),
					TM_FFT_GetFromBuffer(&FFT, bar), FFT_COLOR, FFT_BACKGROUND);
			for (bar = 1; bar < TM_FFT_GetFFTSize(&FFT)/2; bar++)
			{
				/* Draw FFT results */
				DrawBar(fft_pos[0][0]+margin + 2 * bar,
						fft_pos[1][1]-margin,
						FFT_BAR_MAX_HEIGHT,
						TM_FFT_GetMaxValue(&FFT),
						TM_FFT_GetFromBuffer(&FFT, bar),
						FFT_COLOR,
						FFT_BACKGROUND
						);
				RMS += 2.0 * pow(2.0*TM_FFT_GetFromBuffer(&FFT, bar)/n,2);
			}
			/* Show RMS for measured signal */
			switch (mc)
						{
						case 0:
							sprintf(m, "L1: %2.2f ", (Uref * sqrt(RMS)/res)*current_calc);
							break;
						case 1:
							sprintf(m, "L2: %2.2f ", (Uref * sqrt(RMS)/res)*current_calc);
							break;
						case 2:
							sprintf(m, "L3: %2.2f ", (Uref * sqrt(RMS)/res)*current_calc);
							break;
						case 3:
							sprintf(m, "L1: %2.2f ", (Uref * sqrt(RMS)/res)*current_calc);
							break;
						}
						TM_ILI9341_Puts(m_pos[0], m_pos[1], m, &TM_Font_11x18, M_COLOR, M_BACKGROUND);
		}
	}
}

/* Sampling */
void TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        fft_flag = TM_FFT_AddToBuffer(&FFT, ADC_GetConversionValue(uADC) - DC_bias);
    }
}

/* Signal switching on button press */
void EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        SignalSwitch();
    }
}

void SignalSwitch()
{
	uint16_t mask[] = { 0x2000, 0xA000, 0x8000}; //L1->L2, L2->L3, L3->L1
	uint16_t PFval = GPIO_ReadInputData(GPIOF); //PF register value

	if (mc == 3)
		mc = 0;

	PFval ^= mask[mc];
	GPIO_Write(GPIOF, PFval);
	mc++;
}

void DrawBar(uint16_t bottomX, uint16_t bottomY, uint16_t maxHeight, uint16_t maxValue, float32_t value, uint16_t foreground, uint16_t background) {
	uint16_t height;
	height = (uint16_t)((float32_t)value / (float32_t)maxValue * (float32_t)maxHeight);
	if (height == maxHeight) {
		TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
	} else if (height < maxHeight) {
		TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
		TM_ILI9341_DrawLine(bottomX, bottomY - height, bottomX, bottomY - maxHeight, background);
	}
}

void MainInit()
{
	/* Initialize system */
	SystemInit();

	/* Initialize timer */
	JK_TIMER_Init();

	/* Initialize ADC, PA4 is used */
	TM_ADC_Init(uADC, uADC_CHANNEL);

	/* Init FFT */
	TM_FFT_Init_F32(&FFT, FFT_SIZE, 0);
	TM_FFT_SetBuffers_F32(&FFT, Input, Output);

	/* Initialize LCD */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);

	/* Init MUX and IRQs */
	JK_SWITCH_Init();
	JK_MUX_Init();
	JK_IRQ_Init();
}
