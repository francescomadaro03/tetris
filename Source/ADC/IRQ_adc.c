/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "adc.h"
#include "led/led.h"
#include "timer/timer.h"
#include "tetris/backend.h"
/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

void ADC_IRQHandler(void) {
  	
	unsigned short AD_value;
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
	AD_value = AD_current*5/0xFFF;
  if(AD_value != AD_last){
		LED_Off(AD_last);	  // ad_last : AD_max = x : 5
		LED_On(AD_value);	// ad_current : AD_max = x : 5
		
		HandleTimerSpeed(AD_current*5/0xFFF);
		
		
		
		AD_last = AD_value;
  }	
}
