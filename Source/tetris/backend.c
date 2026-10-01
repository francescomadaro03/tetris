#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "timer.h"
#include "LPC17xx.h"
#include "backend.h"
#include "frontend.h"
#include "GLCD.h"
#include "collision_header.h"

#include "ADC/adc.h"

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

extern uint32_t CurrentSpeed;
extern uint32_t DoubleSpeed;

volatile char PowerUpFlag = '0';
volatile uint8_t FoundPowerUp = 0;
volatile uint32_t SavedSpeed;
uint8_t SlowedDown = 0;
uint8_t ClearedLinesOnce = 0;
uint8_t ClearedLineFlag;
volatile uint8_t PowerUpMultiplier = 1;
volatile uint8_t MalusMultiplier = 1;


void DrawPowerup_S(uint16_t x0, uint16_t y0) {
	DrawSquare(x0, y0, Black);
	GUI_Text(x0+4, y0, (uint8_t *) "S", White, Black);
	LCD_DrawLine(x0,y0,x0+14,y0, White);
	LCD_DrawLine(x0,y0+14,x0+14,y0+14, White);
	LCD_DrawLine(x0,y0,x0,y0+14, White);
	LCD_DrawLine(x0+14,y0,x0+14,y0+14, White);
}

void DrawPowerup_L(uint16_t x0, uint16_t y0) {
	DrawSquare(x0, y0, Black);
	GUI_Text(x0+4, y0, (uint8_t *) "L", White, Black);
	LCD_DrawLine(x0,y0,x0+14,y0, White);
	LCD_DrawLine(x0,y0+14,x0+14,y0+14, White);
	LCD_DrawLine(x0,y0,x0,y0+14, White);
	LCD_DrawLine(x0+14,y0,x0+14,y0+14, White);
}

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
	init_timer(0, CurrentSpeed);
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
		uint16_t MultipleOfFour = CanceledRows / 4;
		uint16_t NotMultiple = CanceledRows % 4;
		score += (MultipleOfFour*600);
		score += NotMultiple*100;
	
	}
	else {
		score += 10;
	}

	GUI_Text(170, 25, (uint8_t *) ScoreToString(score), White, Black);
	GUI_Text(170, 100, (uint8_t *) ScoreToString(clearedLinesCount), White, Black);
		
	if(clearedLinesCount >= 5*PowerUpMultiplier && SingleTetronim == 0){
		PowerUpsManagement();
		PowerUpMultiplier++;
	}
	
	if (clearedLinesCount>= MalusMultiplier*10 && SingleTetronim == 0){
		uint8_t triggerGameOver = RandomMalus(Highest_Y);
		if (triggerGameOver == 1){
			GameOver();
		}
		MalusMultiplier++;
	}
	

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
	
	//this function will be extended to handle management of power ups
	char *p = &GAMESTATE[row][0];
	uint8_t i;

	
	for(i = 0; i<10; i++, p++){
		if(*p == '0'){
			return 0;
		
		}

	}
	
	return 1;
}

uint8_t CheckPowerUpInFullLine(uint8_t row){
	char *p = &GAMESTATE[row][0];
	uint8_t i;
	
	char value = *p;
	
	for(i = 0; i<10; i++, p++){
		
		if(*p == 'L'){
			PowerUpFlag = 'L';
			return 1;
		}
		if(*p == 'S'){
			PowerUpFlag = 'S';
			
			return 1;
		}

	
	}
	return 0;
	

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
		if(color_string == 'L'){
			DrawPowerup_L(OnScreenX, OnScreenY);
		
		}
		else if(color_string == 'S'){
			DrawPowerup_S(OnScreenX, OnScreenY);
		}
		else{
			color_code = FindColorCodeFromColor(color_string);
			DrawSquare(OnScreenX, OnScreenY, color_code);
		}
	

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
	
	FoundPowerUp = 0;
	
	for(i = row; i<20; i++){
		FlagValue = CheckFullRow(i);
		
		if(FlagValue == 1 && FoundPowerUp == 0){
			FoundPowerUp = CheckPowerUpInFullLine(i);
			
		}
		
		
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
	ClearedLineFlag = 1;
	Highest_Y += FullRows-1;
	
	
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
	

	
	if(PowerUpFlag == 'L' && ClearedLinesOnce == 0){
		ClearHalfField();
	}
	else if(PowerUpFlag == 'S'){
		SlowDownGame();	
	}
	if(ClearedLinesOnce == 1){
		ClearedLinesOnce = 0;
	
	}
	

	
	
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


void swap(uint8_t *a, uint8_t *b){
	uint8_t temp = *a;
	*a = *b;
	*b = temp;
}

uint8_t * ArrayRandomifier(void){
	static uint8_t BASE_ARRAY[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	uint16_t BASE_STATE = InitialState; uint16_t State;
	uint8_t i, index;
	for(i = 9; i > 2; i--){
		BASE_STATE = LFSR_Random32(BASE_STATE);
		index = BASE_STATE % (i+1);
		swap(&BASE_ARRAY[i], &BASE_ARRAY[index]);
	
	}
	
	
	return &BASE_ARRAY[0];

}

uint8_t AddLastLine(uint8_t * LAST_ROW, uint8_t Highest_Y){
	if((Highest_Y - 1) < 0){
		return 1; //nothing is done: the function ends here, gameover state will be triggered
	}
	uint8_t i;
	for(i = Highest_Y - 1; i<20; i++){
		memmove(
			GAMESTATE[i-1],
			GAMESTATE[i],
			10*sizeof(char)
			);
	}
	memset(GAMESTATE[19], '0', 10*sizeof(char));
	memcpy(GAMESTATE[19], LAST_ROW, 10*sizeof(char));
	return 0;
}


uint8_t RandomMalus(uint8_t Highest_Y){
	uint8_t * MalusRowPositions = ArrayRandomifier();
	uint8_t LAST_ROW[10] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	uint8_t i, triggerGameOverState;
	Highest_Y -= 1;
	
	for(i = 9; i>2; i--){
		LAST_ROW[*MalusRowPositions] = 'G';
		MalusRowPositions++;
	}
	
	triggerGameOverState = AddLastLine(LAST_ROW, Highest_Y);
	if(triggerGameOverState == 1){
		DrawFieldLine(Highest_Y);
	}
	
	if(triggerGameOverState == 0){
		for(i = Highest_Y; i<20; i++){
			DrawFieldLine(i);
		}
	}
	return triggerGameOverState;
	
	

}

/*==================================================================================
Here we will handle the logic to speed up the game
===================================================================================*/

void HandleTimerSpeed(uint8_t PotSpeed){
	if(SlowedDown == 0){
		if(PotSpeed == 0) {
			PotSpeed = 1;
		}
		
		if(PotSpeed > 5){
			PotSpeed = 5;
		}
		
		uint32_t BaseSpeed = 0x17D7840;
		disable_timer(0);
		LPC_TIM0 -> TC = 0;
		LPC_TIM0 -> MR0 = BaseSpeed/PotSpeed;
		enable_timer(0);
		
		CurrentSpeed = BaseSpeed/PotSpeed;
		DoubleSpeed = CurrentSpeed >> 1;
	}

	

}



void PowerUpsManagement(void){
	TETRONIM SpecialBlock = SpecialBlockDefinition();
	volatile uint16_t RandomState = InitialState;
	volatile uint16_t NextState = LFSR_Random32(RandomState);
	
	//per prendere le righe occupate faccio 20 - Highest_Y per trovare il valore di righe. poi sommo quel valore a Highest_y per avere la riga giusta
	
	volatile uint8_t OccupiedRows = 19 - Highest_Y;
	volatile uint8_t GameStateRow = (NextState % OccupiedRows) + Highest_Y;
	NextState = LFSR_Random32(NextState);
	uint8_t GameStateCol = NextState % 10;
	char ChosenPlace = GAMESTATE[GameStateRow][GameStateCol];
	uint8_t trials = 0;
	
	while(ChosenPlace == '0' || trials <=	200){
		trials ++;
		NextState = LFSR_Random32(NextState);
		GameStateRow = (NextState % OccupiedRows) + Highest_Y;
		NextState = LFSR_Random32(NextState);
		GameStateCol = NextState % 10;
		ChosenPlace = GAMESTATE[GameStateRow][GameStateCol];
	}
	
	GAMESTATE[GameStateRow][GameStateCol] = SpecialBlock.SPECIAL_BLOCK;
	char debugVal = GAMESTATE[GameStateRow][GameStateCol];
	uint16_t Mapped_X = (GameStateCol * 15) + 10;
	uint16_t Mapped_Y = (GameStateRow * 15) + 10;
	
	DrawSquare(Mapped_X, Mapped_Y, Black);
	if(SpecialBlock.SPECIAL_BLOCK == 'L'){
		DrawPowerup_L(Mapped_X, Mapped_Y);

	}
	else{
		DrawPowerup_S(Mapped_X, Mapped_Y);
	}
	
	
	

}



void UpdateClearedField(uint16_t HalfOccupiedField){ 
	UpdateGAMESTATE(Highest_Y, HalfOccupiedField, 19); 
} 

void ClearHalfField(void){ 
	uint16_t Half_HighestY = (19 - Highest_Y) >> 1; 
	uint16_t HalfOccupiedField = 19 - Half_HighestY; 
	PowerUpFlag = '0'; UpdateClearedField(HalfOccupiedField); 
}

void SlowDownGame(void){
	SlowedDown = 1;
	if(CurrentSpeed != 0x17D7840){
		SavedSpeed = CurrentSpeed;
		CurrentSpeed = 0x17D7840;
		
		//slow down part
		disable_timer(0);
		LPC_TIM0 -> MR0 = CurrentSpeed;
		reset_timer(0);
		//timer part to handle 15 sec specifications
		init_timer(3, 0x165A0BC0);
		enable_timer(3);
	}
		
		


}