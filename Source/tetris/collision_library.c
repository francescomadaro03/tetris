#include "frontend.h"
#include "backend.h"
#include "collision_header.h"
#include "GLCD.h"

/* 
=============================================================
C MODULE to handle collisions in the map
=================================================================

*/

//DEFINITION OF POSITION MATRICE

const int I_SHAPE[2][4][2] =  {{{0,1}, {1,1}, {2,1}, {3,1}}, {{0,4},{0,4},{0,4},{0,4}}};
const int I_LEN[2] = {4,1};

const int O_SHAPE[2][4][2] = {{{0,2}, {1,2}, {0,2}, {1,2}}, {{0,2}, {1,2}, {0,2}, {1,2}}};
const int O_LEN[2] = {2,2};

const int T_SHAPE[2][4][2] = {{{0,1},{1,2}, {2,1}, {2,1}}, {{0,2}, {1,3}, {1,3}, {1,3}}};
const int T_LEN[2] = {3,2};	

const int J_SHAPE[2][4][2] = {{{0,3},{1,3}, {1,3}, {1,3}}, {{0,2}, {1,2}, {2,2}, {2,2}}};
const int J_LEN[2] = {2,3};

const int L_SHAPE[2][4][2] = {{{0,3},{1,3}, {1,3}, {1,3}}, {{0,2}, {1,1}, {2,1}, {2,1}}};
const int L_LEN[2] = {2,3};

const int S_SHAPE[2][4][2] = {{{0,2}, {1,2}, {2,1}, {2,1}}, {{0,2}, {1,3}, {1,3}, {1,3}}};
const int S_LEN[2] = {3,2};


const int Z_SHAPE[2][4][2] = {{{0,1}, {1,2}, {2,2}, {2,2}}, {{0,3}, {1,2}, {0,3}, {0,3}}};
const int Z_LEN[2] = {3,2};

void UpdateState_I(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	char str[2];
	str[0] = t.COLOR;
	str[1] = '\0';
	
	if(t.ROTATE == 0){

		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y][x+2] = t.COLOR;
		GAMESTATE[y][x+3] = t.COLOR;
	
	}
	else if(t.ROTATE == 1) {
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+2][x] = t.COLOR;
		GAMESTATE[y+3][x] = t.COLOR;
	
	}

}

void UpdateState_O(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	
	GAMESTATE[y][x] = t.COLOR;
	GAMESTATE[y+1][x] = t.COLOR;
	GAMESTATE[y][x+1] = t.COLOR;
	GAMESTATE[y+1][x+1] = t.COLOR;

}

void UpdateState_T(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	
	if(t.ROTATE == 0){
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y][x+2] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
	
	}
	else if(t.ROTATE == 1){
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
		GAMESTATE[y+2][x+1] = t.COLOR;
	}

}

void UpdateState_J(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	
	if(t.ROTATE == 0){
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
		GAMESTATE[y+2][x+1] = t.COLOR;
		GAMESTATE[y+2][x] = t.COLOR;
	
	}
	else if(t.ROTATE == 1){
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
		GAMESTATE[y+1][x+2] = t.COLOR;
	}


}

void UpdateState_L(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	
	if(t.ROTATE == 0){
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+2][x] = t.COLOR;
		GAMESTATE[y+2][x+1] = t.COLOR;
	
	}
	else if(t.ROTATE == 1){
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y][x+2] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
	}



}
void UpdateState_S(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	
	if(t.ROTATE == 0){
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y][x+2] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
	
	}
	else if(t.ROTATE == 1){
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
		GAMESTATE[y+2][x+1] = t.COLOR;
	}

}

void UpdateState_Z(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	
	if(t.ROTATE == 0){
		GAMESTATE[y][x] = t.COLOR;
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
		GAMESTATE[y+1][x+2] = t.COLOR;
	
	
	}
	else if(t.ROTATE == 1){
		GAMESTATE[y][x+1] = t.COLOR;
		GAMESTATE[y+1][x+1] = t.COLOR;
		GAMESTATE[y+1][x] = t.COLOR;
		GAMESTATE[y+2][x] = t.COLOR;
	}

}

uint8_t CheckBoundariesAndUpdate(TETRONIM t, const int POSITION_MATRIX[][4][2], const int LEN[2]){
	
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;
	uint8_t ReturnValue = 0;
	uint8_t RotationValue = t.ROTATE;
	int i, cx, cy, array_length;
	
	
	for(i=0; i< LEN[t.ROTATE]; i++){
		cx = x + POSITION_MATRIX[RotationValue][i][0];
		cy = y + POSITION_MATRIX[RotationValue][i][1];
		if(cy == 20 || GAMESTATE[cy][cx] != '0'){
			ReturnValue = 1;
		}
}

	return ReturnValue;
}


uint8_t CheckAndUpdateState(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;

	uint8_t ReturnValue = 0;
	int LEN; int LEN_ROT;
	

	
	
	switch(t.TETRONIM_TYPE){
		case 'Z':
			LEN = 4;
			ReturnValue = CheckBoundariesAndUpdate(t, Z_SHAPE, Z_LEN);
			if(ReturnValue == 1){
				UpdateState_Z(t);
			}
			break;
		case 'I': 
			LEN = 4;
			ReturnValue = CheckBoundariesAndUpdate(t, I_SHAPE, I_LEN);
			if(ReturnValue == 1){
				UpdateState_I(t);
			}
			break;
		case 'O':
			LEN = 2;
			ReturnValue = CheckBoundariesAndUpdate(t, O_SHAPE, O_LEN);
			if(ReturnValue == 1){
				UpdateState_O(t);
			}
			break;
		case 'T':
			LEN = 4;
			ReturnValue = CheckBoundariesAndUpdate(t, T_SHAPE, T_LEN);
			if(ReturnValue == 1){
				UpdateState_T(t);
			}
			break;
		case 'J':
			LEN = 4;
			ReturnValue = CheckBoundariesAndUpdate(t, J_SHAPE, J_LEN);
			if(ReturnValue == 1){
				UpdateState_J(t);
			}
			break;
		case 'L':
			LEN = 4;
			ReturnValue = CheckBoundariesAndUpdate(t, L_SHAPE, L_LEN);
			if(ReturnValue == 1){
				UpdateState_L(t);
			}
			break;
		case 'S':
			LEN = 4;
			ReturnValue = CheckBoundariesAndUpdate(t, S_SHAPE, S_LEN);
			if(ReturnValue == 1){
				UpdateState_S(t);
			}
			break;
		default:
			break;
	
	
	}
	
	
	return ReturnValue;


}


//DEFINITION OF HORIZONTAL POSITION MATRIX

const int I_HORIZONTAL_SHAPE_L[2][4][2] = {{{-1,0}, {-1,0}, {-1,0}, {-1,0}}, {{-1,0},{-1,1},{-1,2},{-1,3}}};
const int I_HORIZONTAL_SHAPE_R[2][4][2] = {{{4,0}, {4,0}, {4,0}, {4,0}}, {{1,0},{1,1},{1,2},{1,3}}};
const int I_HORIZONTAL_LEN[2] = {1,4};

const int O_HORIZONTAL_SHAPE_L[2][4][2] = {{{-1,0}, {-1,1}, {-1,0}, {-1,1}},{{-1,0},{-1,1},{-1,0},{-1,1}}};
const int O_HORIZONTAL_SHAPE_R[2][4][2] = {{{2,0}, {2,1}, {2,0}, {2,1}}, {{2,0}, {2,1}, {2,0}, {2,1}}};
const int O_HORIZONTAL_LEN[2] = {2,2};

const int T_HORIZONTAL_SHAPE_L[2][4][2] = {{{-1,0}, {0,1}, {-1,0}, {-1,0}}, {{0,0},{-1,1},{0,2},{0,2}}};
const int T_HORIZONTAL_SHAPE_R[2][4][2] = {{{3,0}, {2,1}, {3,0}}, {{2,0}, {2,1}, {2,2}}};
const int T_HORIZONTAL_LEN[2] = {2,3};

const int J_HORIZONTAL_SHAPE_L[2][4][2] = {{{0,0}, {0,1}, {-1,2},{0,0}}, {{-1,0},{-1,1},{-1,0},{-1,0}}};
const int J_HORIZONTAL_SHAPE_R[2][4][2] = {{{2,0}, {2,1}, {2,2},{2,0}}, {{1,0}, {3,1}, {1,2},{1,0}}};
const int J_HORIZONTAL_LEN[2] = {3,2};

const int L_HORIZONTAL_SHAPE_L[2][4][2] = {{{-1,0}, {-1,1}, {-1,2}, {-1,0}}, {{-1,1},{1,0},{-1,1}, {1,0}}};
const int L_HORIZONTAL_SHAPE_R[2][4][2] = {{{1,0}, {1,1}, {2,2}, {1,0}}, {{3,0}, {3,1}, {3,0}, {3,1}}};
const int L_HORIZONTAL_LEN[2] = {3,2};

const int S_HORIZONTAL_SHAPE_L[2][4][2] = {{{0,0}, {-1,1}, {0,0}, {-1,1}}, {{-1,0},{-1,1},{0,2}, {-1,0}}};
const int S_HORIZONTAL_SHAPE_R[2][4][2] = {{{3,0}, {2,1}, {3,0}, {2,1}}, {{1,0}, {2,1}, {2,2}, {2,1}}};
const int S_HORIZONTAL_LEN[2] = {2,3};

const int Z_HORIZONTAL_SHAPE_L[2][4][2] = {{{-1,0}, {0,1}, {-1,0}, {0,1}}, {{0,0},{-1,1},{-1,2}, {0,0}}};
const int Z_HORIZONTAL_SHAPE_R[2][4][2] = {{{2,0}, {3,1}, {2,0}, {3,1}}, {{2,0}, {2,1}, {1,2}, {2,0}}};
const int Z_HORIZONTAL_LEN[2] = {2,3};

uint8_t CheckHorizontalState(TETRONIM t){
	int x = (t.COORD[0] - 10) / 15;
	int y = (t.COORD[1] - 10) / 15;

	uint8_t ReturnValue = 0;
	
	switch(t.TETRONIM_TYPE){
		case 'Z':
			ReturnValue = CheckBoundariesAndUpdate(t, Z_HORIZONTAL_SHAPE_L, Z_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, Z_HORIZONTAL_SHAPE_R, Z_HORIZONTAL_LEN);
		}
			break;
		case 'I': 
			ReturnValue = CheckBoundariesAndUpdate(t, I_HORIZONTAL_SHAPE_L, I_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, I_HORIZONTAL_SHAPE_R, I_HORIZONTAL_LEN);
		}
			break;
		case 'O':
			ReturnValue = CheckBoundariesAndUpdate(t, O_HORIZONTAL_SHAPE_L, O_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, O_HORIZONTAL_SHAPE_R, O_HORIZONTAL_LEN);
		}
			break;
		case 'T':
			ReturnValue = CheckBoundariesAndUpdate(t, T_HORIZONTAL_SHAPE_L, T_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, T_HORIZONTAL_SHAPE_R, T_HORIZONTAL_LEN);
		}
			break;
		case 'J':
			ReturnValue = CheckBoundariesAndUpdate(t, J_HORIZONTAL_SHAPE_L, J_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, J_HORIZONTAL_SHAPE_R, J_HORIZONTAL_LEN);
		}
			break;
		case 'L':
			ReturnValue = CheckBoundariesAndUpdate(t, L_HORIZONTAL_SHAPE_L, L_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, L_HORIZONTAL_SHAPE_R, L_HORIZONTAL_LEN);
		}
			break;
		case 'S':
			ReturnValue = CheckBoundariesAndUpdate(t, S_HORIZONTAL_SHAPE_L, S_HORIZONTAL_LEN);
		if (ReturnValue == 0) {
			ReturnValue = CheckBoundariesAndUpdate(t, S_HORIZONTAL_SHAPE_R, S_HORIZONTAL_LEN);
		}	
		break;
		default:
			break;
	}
	return ReturnValue;
}