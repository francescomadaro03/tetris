/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "GLCD.h" 
#include "TouchPanel.h"
#include "tetris/backend.h"
#include "tetris/frontend.h"
#include "led/led.h"
#include <stdio.h> /*for sprintf*/
#include "music/music.h"

uint32_t TIMER_SPEED = 0x17D7840;
uint32_t TIMER_DOUBLE = (0x1312D0 >> 1);
volatile uint8_t Highest_Y;


uint16_t SinTable[45] =                                       
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/



void TIMER0_IRQHandler (void)
{
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
	static uint8_t collision = 0;
	collision = CheckCollisions(State);
	uint16_t Y = ((State.COORD[1] - 10) / 15);
	if(collision == 1){
		if(Y < Highest_Y){
			Highest_Y = Y;
		}
		if(Highest_Y - State.VERTICAL_LENGTH < 0){
			GameOver();
		}
		else {
			ComputePoints(0,1);
			CheckGAMESTATE(Highest_Y);
			collision = 0;
			NewTetroid();
		}

	}
	else {
		State = VerticalMovementHandler(State);
	}

  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	

  static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks]*0.7;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45){
		sineticks=0;
	}

	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;

}


void TIMER2_IRQHandler (void){
	
	InitControllingTimer();
	playNote();
	LPC_TIM2->IR = 1;


}
/******************************************************************************
**                            End Of File
******************************************************************************/
