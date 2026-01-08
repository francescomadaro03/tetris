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

uint32_t TIMER_SPEED = 0x17D7840;
uint32_t TIMER_DOUBLE = (0x1312D0 >> 1);
volatile uint8_t Highest_Y;


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
	

  LPC_TIM1->IR = 1;			/* clear interrupt flag */
	
	
	if((LPC_GPIO1 -> FIOPIN & (1<<29)) == 0){
		disable_timer(0);
		State = RotateCurrentTetronim(State);
		MovementInit();

	}
	
	else if((LPC_GPIO1 -> FIOPIN & (1<<28)) == 0){
		disable_timer(0);
		State = HorizontalMovementHandler(State, 'R');
		enable_timer(0);
		
	}
	else if((LPC_GPIO1 -> FIOPIN & (1<<27)) == 0){		
		disable_timer(0);
		State = HorizontalMovementHandler(State, 'L');
		enable_timer(0);
		
	}

	
	uint8_t JoystickDown = ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0);

	if (JoystickDown == 1) {
		if (LPC_TIM0->MR0 != TIMER_DOUBLE) {
					disable_timer(0);
					LPC_TIM0->MR0 = TIMER_DOUBLE;
					enable_timer(0);
			}
	} 
	else {
			if (LPC_TIM0->MR0 != TIMER_SPEED) {
					disable_timer(0);
					LPC_TIM0->MR0 = TIMER_SPEED;
					if (LPC_TIM0->TC > LPC_TIM0->MR0)
							LPC_TIM0->TC = 0;
					enable_timer(0);
			}
	}
  return;

}

/******************************************************************************
**                            End Of File
******************************************************************************/
