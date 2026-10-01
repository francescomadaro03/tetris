/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD.h" 
#include "TouchPanel.h"
#include "timer.h"
#include "tetris/frontend.h"
#include "tetris/backend.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "button.h"
#include "music/music.h"
#include "ADC/adc.h"



#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


volatile TETRONIM State;
extern volatile uint8_t Highest_Y;
extern volatile uint8_t GameOverFlag;
volatile uint8_t StartingGame = 1;
volatile uint32_t CurrentSpeed;
volatile uint32_t DoubleSpeed;


int main(void)
{
	
  SystemInit();
	CurrentSpeed = 0x17D7840;
	DoubleSpeed = CurrentSpeed >> 1;
	Highest_Y = 19;
	GameOverFlag = 0;
 //UPDATE TIMER TO COMPLY WITH LANDTIGER

	joystick_init();
  LCD_Initialization();
	ADC_init();

	ConfigurationProcedureTiming();
	/*enable_timer(1);
	init_timer(1, 0x1312D0);
	enable_timer(1);*/

	
	init_tetris_frontend();
	InitMusicTimer();
	init_RIT(0x1C9C38 << 2); 
	//DAC INITIALIZATION
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);			/* pin 0.26 is AOUT */
	LPC_GPIO0->FIODIR |= (1<<26);					
	
	//InitControllingTimer();
	
	
	
	//NewTetroid();
	

	//LCD_DrawLine(0, 0, 200, 200, White);
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	SCB->SCR |= 0x2;											/* set SLEEPONEXIT */
	
	__ASM("wfi");

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
