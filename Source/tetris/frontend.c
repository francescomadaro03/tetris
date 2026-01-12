#include <stdlib.h>

#include "frontend.h"
#include "backend.h"
#include "GLCD.h"
#include "timer.h"

volatile uint16_t record = 0;
volatile uint16_t score = 0;
volatile uint16_t clearedLinesCount;
volatile uint16_t InitialState;
static uint8_t SpaceAbove = 0;
volatile uint8_t GameOverFlag = 1;
extern uint8_t Highest_Y;
extern int game_paused;
extern uint8_t starting_game;





uint16_t LFSR_Random32(uint16_t state){
	uint16_t feedback =  ((state >> 7) ^ (state >> 4) ^ (state >> 3) ^ (state >> 11)) & 0x01;
	uint16_t new_state = ((state >> 1)) | (feedback << 15);
	
	return new_state;
	
}

char* ScoreToString(uint16_t score){
	static char string[257];
	int i = 0; int last_digit; int j = 0;
	char last_char; 
	unsigned int tmp = score;
	
	if(score == 0){
		string[0] = '0';
		string[1] = '\0';
		return string;
	}
	
	while(tmp > 0){
		last_digit = tmp % 10;
		last_char = last_digit + '0';
		string[i] = last_char;
		tmp = tmp/10;
		i++;
	}
	
	string[i] = '\0'; //string terminator
	
	
	for(; j<i/2; j++){
		tmp = string[j];    //54321'\0'
		string[j] = string[i-1-j];
		string[i-1-j] = tmp;
	
	}
	
	
	
	
	return string;
	
	
	
	
	
}



void init_tetris_frontend(void) {
	clearedLinesCount = 9;
	
	
	
	LCD_Clear(Black);
	LCD_DrawLine(9, 9, 9, 311, White);		//top-left to bottom-left
	LCD_DrawLine(9, 311, 160, 311, White);	//bottom-left to bottom_right
	LCD_DrawLine(160, 311, 160, 9, White);	//bottom_right to top-right
	LCD_DrawLine(160, 9, 9, 9, White);		//top-right to top-left
	
	GUI_Text(170, 10, (uint8_t *) "SCORE:", White, Black);
	GUI_Text(170, 25, (uint8_t *) ScoreToString(score), White, Black);
	
	GUI_Text(170, 40, (uint8_t *) "RECORD:", White, Black);
	GUI_Text(170, 55, (uint8_t *) ScoreToString(record), White, Black);
	
	GUI_Text(170, 70, (uint8_t *) "CLEARED", White, Black);
	GUI_Text(170, 85, (uint8_t *) "LINES:", White, Black);
	GUI_Text(170, 100, (uint8_t *) ScoreToString(clearedLinesCount), White, Black);	
	GUI_Text(170, 115, (uint8_t *) "PAUSE", Red, Black);


	
	
	
}

void DrawSquare(uint16_t x0, uint16_t y0, uint16_t color){
	LCD_DrawLine(x0,y0,x0+14,y0, Black);
	LCD_DrawLine(x0,y0+1,x0+14,y0+1, color);
	LCD_DrawLine(x0,y0+2,x0+14,y0+2, color);
	LCD_DrawLine(x0,y0+3,x0+14,y0+3, color);
	LCD_DrawLine(x0,y0+4,x0+14,y0+4, color);
	LCD_DrawLine(x0,y0+5,x0+14,y0+5, color);
	LCD_DrawLine(x0,y0+6,x0+14,y0+6, color);
	LCD_DrawLine(x0,y0+7,x0+14,y0+7, color);
	LCD_DrawLine(x0,y0+8,x0+14,y0+8, color);
	LCD_DrawLine(x0,y0+9,x0+14,y0+9, color);
	LCD_DrawLine(x0,y0+10,x0+14,y0+10, color);
	LCD_DrawLine(x0,y0+11,x0+14,y0+11, color);
	LCD_DrawLine(x0,y0+12,x0+14,y0+12, color);
	LCD_DrawLine(x0,y0+13,x0+14,y0+13, color);
	LCD_DrawLine(x0,y0+14,x0+14,y0+14, Black);
	LCD_DrawLine(x0,y0,x0,y0+14, Black);
	LCD_DrawLine(x0+14,y0,x0+14,y0+14, Black);		
}

void DrawTetroid_I(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	if(rotation == 0){
		DrawSquare(x0,y0, color);
		DrawSquare(x0+15, y0, color);
		DrawSquare(x0+30, y0, color);
		DrawSquare(x0+45, y0, color);
		
	}
	else {
		DrawSquare(x0,y0, color);
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0, y0+30, color);
		DrawSquare(x0, y0+45, color);		
	
	}

	return;
	

}

void DrawTetroid_O(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	DrawSquare(x0, y0, color);
	DrawSquare(x0 + 15, y0, color);
	DrawSquare(x0, y0 + 15, color);
	DrawSquare(x0+15, y0+15, color);
	return;
}

void DrawTetroid_T(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	if(rotation == 0){
		DrawSquare(x0,y0, color);
		DrawSquare(x0+15, y0, color);
		DrawSquare(x0+30, y0, color);
		DrawSquare(x0+15, y0+15, color);
	}
	else {
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0 +15, y0, color);
		DrawSquare(x0 +15, y0+15, color);
		DrawSquare(x0 + 15, y0 +30, color);
	
	
	}
	return;
}


void DrawTetroid_J(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	if (rotation == 0) {
		DrawSquare(x0+15, y0, color);
		DrawSquare(x0+15, y0+15, color);
		DrawSquare(x0+15, y0+30, color);
		DrawSquare(x0, y0+30, color);
	}
		if (rotation == 1) {
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0, y0, color);
		DrawSquare(x0+15, y0+15, color);
		DrawSquare(x0+30, y0+15, color);
	}
		return;
}

void DrawTetroid_L(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	if (rotation == 0) {
		DrawSquare(x0, y0, color);
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0, y0+30, color);
		DrawSquare(x0+15, y0+30, color);	
	}
	if (rotation == 1) {
		DrawSquare(x0, y0+30, color);
		DrawSquare(x0, y0+45, color);
		DrawSquare(x0+15, y0+30, color);
		DrawSquare(x0+30, y0+30, color);	
	}
		return;
}

void DrawTetroid_S(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	if (rotation == 0) {
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0+15, y0+15, color);
		DrawSquare(x0+15, y0, color);
		DrawSquare(x0+30, y0, color);
	}
	if (rotation == 1) {
		DrawSquare(x0, y0, color);
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0+15, y0+15, color);
		DrawSquare(x0+15, y0+30, color);
	}
	return;
}

void DrawTetroid_Z(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation){
	if (rotation == 0) {
		DrawSquare(x0, y0, color);
		DrawSquare(x0+15, y0, color);
		DrawSquare(x0+15, y0+15, color);
		DrawSquare(x0+30, y0+15, color);
	}
	if (rotation == 1) {
		DrawSquare(x0+15, y0, color);
		DrawSquare(x0+15, y0+15, color);
		DrawSquare(x0, y0+15, color);
		DrawSquare(x0, y0+30, color);
	}
	return;
}


char RandomTetroidGenerator(void){
	uint16_t random; char tetroid;
	InitialState = LFSR_Random32(InitialState);
	random = InitialState % 7;
	switch(random){
		case 0:
			tetroid = 'I';
			break;
		case 1:
			tetroid = 'O';
			break;
		case 2:
			tetroid = 'T';
			break;
		case 3:
			tetroid = 'J';
			break;
		case 4:

			tetroid = 'L';
			break;
		case 5:
			tetroid = 'S';
			break;
		case 6:
			tetroid = 'Z';
			break;
		default:
			break;
	
	}
	return tetroid;
	
}
	




TETRONIM TetroidDrawer(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation, char typeT){

	TETRONIM T;
	
	switch(typeT){
		case 'I':
			DrawTetroid_I(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'I';
			T.ROTATE = rotation;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;
			T.HORIZONTAL_LENGTH = 4;
			T.HORIZONTAL_ROTATE_LENGTH = 1;
			T.VERTICAL_LENGTH = 1;
			break;
		case 'O':
			DrawTetroid_O(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'O';
			T.ROTATE = 0;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;
			T.HORIZONTAL_LENGTH = 2;
			T.HORIZONTAL_ROTATE_LENGTH = 2;
			T.VERTICAL_LENGTH = 2;
			break;
		case 'T':
			DrawTetroid_T(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'T';
			T.ROTATE = 0;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;
			T.HORIZONTAL_LENGTH = 3;
			T.HORIZONTAL_ROTATE_LENGTH = 2;
			T.VERTICAL_LENGTH = 2;

			break;
		case 'J':
			DrawTetroid_J(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'J';
			T.ROTATE = 0;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;
			T.HORIZONTAL_LENGTH = 2;
			T.HORIZONTAL_ROTATE_LENGTH = 3;
			T.VERTICAL_LENGTH = 3;

			break;
		case 'L':
			DrawTetroid_L(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'L';
			T.ROTATE = 0;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;
		
			T.HORIZONTAL_LENGTH = 2;
			T.HORIZONTAL_ROTATE_LENGTH = 3;
			T.VERTICAL_LENGTH = 3;
			

			break;
		case 'S':
			DrawTetroid_S(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'S';
			T.ROTATE = 0;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;
			T.HORIZONTAL_LENGTH = 3;
			T.HORIZONTAL_ROTATE_LENGTH = 2;
			T.VERTICAL_LENGTH = 2;
			break;
		case 'Z':
			DrawTetroid_Z(x0, y0, color, rotation);
			T.TETRONIM_TYPE = 'Z';
			T.ROTATE = 0;
			T.COLOR = FindColorFromCode(color);
			T.COLOR_CODE = color;
			T.COORD[0] = x0;
			T.COORD[1] = y0;			
  		T.HORIZONTAL_LENGTH = 3;
			T.HORIZONTAL_ROTATE_LENGTH = 2;
			T.VERTICAL_LENGTH = 2;
			break;
		default:
			break;
	
	}
	char str[2];

	return T;
	
}

uint16_t FindColorCodeFromColor(char color){
	uint16_t ReturnCode = 0;
		switch(color){
			case 'W':
				ReturnCode = 0xFFFF;
				break;
			case 'B':
				ReturnCode = 0x0000 ;
				break;
			case 'G':
				ReturnCode = 0xF7DE;
				break;
			case '1':
				ReturnCode = 0x001F; //it stands for Blue1
				break;
			case  '2':
				ReturnCode = 0x051F; //it stands for Blue2
				break;
			case 'R':
				ReturnCode = 0xF800;
				break;
			case 'M':
				ReturnCode = 0xF81F;
				break;
			case 'g':
				ReturnCode = 0x07E0; //it stands for green
				break;
			case 'C':
				ReturnCode = 0x7FFF;
				break;
			case 'Y':
				ReturnCode = 0xFFE0;
				break;
			case 'm':
				ReturnCode = 0x8010; //MALUS color
			default:
				break;
		}
		return ReturnCode;



}
char FindColorFromCode(uint16_t color){
		char ReturnCode;
		switch(color){
			case 0xFFFF:
				ReturnCode = 'W';
				break;
			case 0x0000:
				ReturnCode = 'B';
				break;
			case 0xF7DE:
				ReturnCode = 'G';
				break;
			case 0x001F:
				ReturnCode = '1'; //it stands for Blue1
				break;
			case 0x051F:
				ReturnCode = '2'; //it stands for Blue2
				break;
			case 0xF800:
				ReturnCode = 'R';
				break;
			case 0xF81F:
				ReturnCode = 'M';
				break;
			case 0x07E0:
				ReturnCode = 'g'; //it stands for green
				break;
			case 0x7FFF:
				ReturnCode = 'C';
				break;
			case 0xFFE0:
				ReturnCode = 'Y';
				break;
		
		}
		return ReturnCode;
}


uint16_t Find_Color_From_Type(char tetroid_type) {
	uint16_t color_code = 0x0000;
	switch(tetroid_type){
		case 'I':
			color_code = 0xFFE0;
			break;
		case 'Z':
			color_code = 0x7FFF;
			break;
		case 'O':
			color_code = 0x07E0;
			break;
		case 'T':
			color_code = 0xF81F;
			break;
		case 'J':
			color_code = 0xF800;
			break;
		case 'L':
			color_code = 0x051F;
			break;
		case 'S':
			color_code = 0x001F;
			break;
		default:
			break;
}
	return color_code;
}

void GameOver(void){
	 
	disable_timer(0);
	GameOverFlag = 1;
	clearedLinesCount = 0;
	Highest_Y = 19;
	Reset_GAMESTATE();
	LCD_Clear(Black);
	GUI_Text(50, 100, (uint8_t *) "YOU LOST", Red, White);
	GUI_Text(50, 115, (uint8_t *) "PRESS KEY1", Red, White);
	GUI_Text(50, 130, (uint8_t *) "TO PLAY AGAIN", Red, White);
	
	game_paused = 1;
	starting_game = 1;
	if(score > record){
		record = score;
	
	}
	score = 0;
	
	
}




void NewTetroid(void) {
	char typeT;
	uint16_t color_code;
	typeT = RandomTetroidGenerator();
	color_code = Find_Color_From_Type(typeT);
	State = TetroidDrawer(55, 10, color_code, 0, 'I');
	MovementInit();	
	
	return;
}


