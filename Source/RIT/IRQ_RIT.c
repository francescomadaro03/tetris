/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "/led/led.h"
#include "/timer.h"
#include "/GLCD/GLCD.h"
#include "tetris/backend.h"
#include "tetris/frontend.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;
extern uint8_t GameOverFlag;
volatile uint8_t starting_game = 1;
volatile int game_paused = 1;


void RIT_IRQHandler (void)
	
{	

	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */


	/*
	==================================================
	KEY1 INTERRUPT HANDLING
	==================================================
	*/
	
	static volatile int key1_pressed = 0;
	volatile uint32_t CurrentSpeed = 0x17D7840;
	volatile uint32_t DoubleSpeed = CurrentSpeed >> 1;
	volatile int KEY1_PIN_VAL = (LPC_GPIO2->FIOPIN & (1 << 11));
	volatile uint8_t HorizontalFlag;

		
	if(KEY1_PIN_VAL == 0){
		if(key1_pressed == 0){
			key1_pressed = 1;
			if(GameOverFlag == 1){
				LPC_GPIO2->FIODIR   &= ~(1 << 12); // KEY2
				GameOverFlag = 0;
				init_tetris_frontend();
			
			}
			else if(game_paused == 0){
				disable_timer(0);
				GUI_Text(170, 115, (uint8_t *) "PAUSE", Red, Black);
				game_paused = 1;
				
			}
			else if(game_paused == 1 && starting_game == 1){
				starting_game = 0;
				game_paused = 0;
				GUI_Text(170, 115, (uint8_t *) "PAUSE", Black, Black);
				NewTetroid();
			
			}
			else{
				GUI_Text(170, 115, (uint8_t *) "PAUSE", Black, Black);
				enable_timer(0);
				game_paused = 0;
			}
		
		}
	}
	else {
		key1_pressed = 0;
	
	}
	/*
	==================================================
	KEY2 INTERRUPT HANDLING
	==================================================
	*/
	
	
	volatile int KEY2_PIN_VALUE =LPC_GPIO2->FIOPIN & (1 << 12);
	volatile int key2_pressed = 0;
	
	if(KEY2_PIN_VALUE == 0 && GameOverFlag == 0){
		if(key2_pressed == 0){
			key2_pressed = 1;
			HardDropTetroid(State);
		}
		else{
			key2_pressed = 0;
		}
	
	}
	
	
	/*================================================================
	JOYSTICK MANAGEMENT
	=================================================================*/
	
	
	uint8_t JOYSTICK_MOVED = 0;
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		if(JOYSTICK_MOVED == 0){
			disable_timer(0);
			State = RotateCurrentTetronim(State);
			MovementInit();
			JOYSTICK_MOVED = 1;
		}
	}
	else if((LPC_GPIO1 -> FIOPIN & (1<<28)) == 0){
			disable_timer(0);
			HorizontalFlag = CheckHorizontalState(State);
			if (HorizontalFlag == 0) {
				State = HorizontalMovementHandler(State, 'R');
			}
			enable_timer(0);
			
		}
		else if((LPC_GPIO1 -> FIOPIN & (1<<27)) == 0){		
			disable_timer(0);
			HorizontalFlag = CheckHorizontalState(State);
			if (HorizontalFlag == 0) {
				State = HorizontalMovementHandler(State, 'L');
			}
			enable_timer(0);
			
		}
	else {
		JOYSTICK_MOVED = 0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		if((LPC_TIM0 -> MR0) != DoubleSpeed){
			disable_timer(0);
			LPC_TIM0->MR0 = DoubleSpeed; 
			LPC_TIM0->TC = 0; 

			enable_timer(0);
		} 
	}
	else {
		if ((LPC_TIM0->MR0) == DoubleSpeed) {
			disable_timer(0);
			LPC_TIM0->MR0 = CurrentSpeed;
			LPC_TIM0->TC = 0;
			enable_timer(0);
	}
}

	
}

/******************************************************************************
**                            End Of File
******************************************************************************/
