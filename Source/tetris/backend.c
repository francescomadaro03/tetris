#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "timer.h"
#include "LPC17xx.h"
#include "backend.h"
#include "frontend.h"
#include "GLCD.h"
#include "collision_header.h"

/* ===================================================================
	 This file handles all gaming logic of the TETRIS GAME.
	 Some other feature related to frontend management are placed
	 in the frontend.c file



	=================================================================== */
	
	
	
/* HERE ALL GLOBAL VARIABLES WILL BE LISTED" */
extern uint16_t InitialState;
extern uint16_t score;
extern uint16_t record;
extern int GameOverFlag;
extern uint8_t Highest_Y;
extern uint16_t clearedLinesCount;


char GAMESTATE[ROWS][COLS] = { \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
    {'0','0','0','0','0','0','0','0','0','0'}, \
		{'0','0','0','0','0','0','0','0','0','0'}, \
		{'0','0','0','0','0','0','0','0','0','0'}
};


void Reset_GAMESTATE(void){
   memset(GAMESTATE, '0', ROWS * COLS * sizeof(char));
}


void ConfigurationProcedureTiming(void){
	uint32_t TimerValue;
	enable_timer(0);
	//now touchpad configuration starts
	TP_Init();
	TouchPanel_Calibrate();
	disable_timer(0); //the timer stops counting as soon as the configuration is done
	TimerValue = LPC_TIM0 -> TC;
	InitialState = (TimerValue >> 16); //the time spent to configure the system becomes the seed for the random generation of tetroids
	
	

}

void MovementInit(void){
	reset_timer(0);
	init_timer(0, (0x17D7840));
	//init_timer(0, 0x1312D0); //to simulate the tetris in the simulator
	enable_timer(0);
	return;
}

TETRONIM VerticalMovementHandler(TETRONIM t){

	
	TetroidDrawer(t.COORD[0], t.COORD[1], Black, t.ROTATE, t.TETRONIM_TYPE);
	TetroidDrawer(t.COORD[0], t.COORD[1] + 15, t.COLOR_CODE, t.ROTATE, t.TETRONIM_TYPE);
	t.COORD[1] = t.COORD[1] + 15;

	//logica per gestire lo stato
	
	return t;
}


TETRONIM HorizontalMovementHandler(TETRONIM t, char direction){
	uint8_t MAPPED_X = (t.COORD[0] - 10) / 15;
	if(direction == 'R'){

		if((t.ROTATE == 0 && MAPPED_X + t.HORIZONTAL_LENGTH < 10) || (t.ROTATE == 1 && MAPPED_X + t.HORIZONTAL_ROTATE_LENGTH < 10)){

			TetroidDrawer(t.COORD[0], t.COORD[1], Black, t.ROTATE, t.TETRONIM_TYPE);
			TetroidDrawer(t.COORD[0] + 15, t.COORD[1], t.COLOR_CODE, t.ROTATE, t.TETRONIM_TYPE);
			t.COORD[0] += 15;
		}
	
	}

	else {
			if(MAPPED_X -1 >= 0) {

				TetroidDrawer(t.COORD[0], t.COORD[1], Black, t.ROTATE, t.TETRONIM_TYPE);
				TetroidDrawer(t.COORD[0]-15, t.COORD[1], t.COLOR_CODE, t.ROTATE, t.TETRONIM_TYPE);
				t.COORD[0] = t.COORD[0] - 15;



			}
	
	}

	

	return t;
}

TETRONIM RotateCurrentTetronim(TETRONIM t){
	
	uint8_t MAPPED_X = (t.COORD[0] - 10) / 15;
	if((MAPPED_X + t.HORIZONTAL_ROTATE_LENGTH < 10)){
		uint8_t RotationValue = !t.ROTATE;
		TetroidDrawer(t.COORD[0], t.COORD[1], Black, t.ROTATE, t.TETRONIM_TYPE);
		TetroidDrawer(t.COORD[0], t.COORD[1], t.COLOR_CODE, RotationValue, t.TETRONIM_TYPE);
		
		t.ROTATE = RotationValue;
	}

	
	return t;



}

uint8_t CheckCollisions(TETRONIM State){
	uint8_t ReturnValue = 0;
	ReturnValue = CheckAndUpdateState(State);	
	return ReturnValue;

}



void ComputePoints(uint16_t CanceledRows, uint8_t SingleTetronim){
	
	GUI_Text(170, 25, (uint8_t *) ScoreToString(score), Black, Black);
	GUI_Text(170, 100, (uint8_t *) ScoreToString(clearedLinesCount), Black, Black);
	clearedLinesCount += CanceledRows;
	
	
	//100 per riga, 10 per piazzamento, 600 se ho tetris
	if(SingleTetronim == 0){
		if(CanceledRows != 4){
		score += (CanceledRows*100);
		}
		else {
			score += 600;
		}
	
	}
	else {
		score += 10;
	}

	GUI_Text(170, 25, (uint8_t *) ScoreToString(score), White, Black);
	GUI_Text(170, 100, (uint8_t *) ScoreToString(clearedLinesCount), White, Black);
	

}

/*
===========================================
HERE STARTS THE LOGIC TO HANDLE THE GAME WIN CONDITIONS
===========================================

*/

//this function will handle the checks and possible
//function calls to the logic to change the field
//if some rows are full. the function will 
//return 0 if no collisions are found
uint8_t CheckFullRow(uint8_t row){
	char *p = &GAMESTATE[row][0];
	uint8_t i;

	
	for(i = 0; i<10; i++, p++){
		if(*p == '0'){
			return 0;
		
		}
		

	}
	
	return 1;
}

void DrawFieldLine(uint8_t row){
	volatile uint16_t OnScreenX;
	volatile uint16_t OnScreenY = (row*15) + 10;
	volatile char color;
	volatile uint16_t i, color_code;
	volatile char color_string;
	
	for(i = 0; i<10; i++){
		OnScreenX = (i*15) + 10;
		DrawSquare(OnScreenX, OnScreenY, 0x0000);
		color_string = GAMESTATE[row][i];
		color_code = FindColorCodeFromColor(color_string);
		DrawSquare(OnScreenX, OnScreenY, color_code);
	}

}


void CheckGAMESTATE(uint8_t row){
	volatile int8_t NotFullBefore = 0;
	volatile int8_t FullRows = 0;
	volatile int8_t FlagValue;
	volatile int8_t ChangeState = 0; //this variable monitors if the program has detected empty lines after full lines
	volatile int8_t FirstIndexFull = 30;
	volatile int8_t LastIndexFull = 30;
	volatile uint8_t i;
	
	for(i = row; i<20; i++){
		FlagValue = CheckFullRow(i);
		if(FlagValue == 1 && ChangeState == 0){
			FirstIndexFull = i;
			FullRows++;
			ChangeState = 1;
		}
		else if(FlagValue == 1 && ChangeState == 1){
			FullRows++;
		}
		else if(FlagValue == 0 && ChangeState == 1){
			LastIndexFull = i-1;
			break;
		}
		else{
			NotFullBefore++;
		}
	}
	
	if(ChangeState == 1 && LastIndexFull == 30){
    LastIndexFull = i - 1;
	}
	
	
	
	if(FirstIndexFull != 30){
		UpdateGAMESTATE(row,FirstIndexFull,LastIndexFull);
	
	}
}

void UpdateGAMESTATE(int8_t row, int8_t FirstIndexFull, int8_t LastIndexFull){
	volatile int8_t FullRows = LastIndexFull - FirstIndexFull + 1; //this variable takes all rows that are not full before the first one
	volatile int8_t i;

	for(i = FirstIndexFull-1; i>=row; i--){
		memmove(GAMESTATE[i + FullRows], GAMESTATE[i], 10);	
	}
	
	i = row;
	
	for(; i< (row+FullRows); i++){
		memset(GAMESTATE[i], '0', 10);
	}

	i = row;
	
	for(; i<=LastIndexFull; i++){
		DrawFieldLine(i);
	}
	
	ComputePoints(FullRows, 0);
	
	
}





void HardDropTetroid(TETRONIM State){
	disable_timer(0);
	uint8_t CollisionFlag = 0;
	uint8_t row;
	while(CollisionFlag == 0){
		State = VerticalMovementHandler(State);
		CollisionFlag = CheckCollisions(State);
		if(CollisionFlag == 1){
			row = (State.COORD[1] - 10) / 15;
			if(Highest_Y - State.VERTICAL_LENGTH < 0){
				GameOver();
			
			}
			else {
			if(row < Highest_Y){
				Highest_Y = row;
			}

			CheckGAMESTATE(Highest_Y);
			ComputePoints(0,1);
			
			}

			
		}
	
	}
	NewTetroid();

}