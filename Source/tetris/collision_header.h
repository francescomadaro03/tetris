#include "frontend.h"
#include "backend.h"

#ifndef __COLLISION_LIBRARY

#define __COLLISION_LIBRARY
uint8_t CheckAndUpdateState(TETRONIM t);
uint8_t CheckBoundariesAndUpdate(TETRONIM t, const int POSITION_MATRIX[][4][2], const int LEN[2]);
uint8_t CheckHorizontalState(TETRONIM t);
void UpdateState_I(TETRONIM t);


#endif

//MATRIX OF POSITIONS

#define CALL_UPDATE(type, ...) UpdateState_##type(__VA_ARGS__)
extern const int I_SHAPE[2][4][2];
extern const int O_SHAPE[2][4][2];
extern const int T_SHAPE[2][4][2];
extern const int J_SHAPE[2][4][2];
extern const int L_SHAPE[2][4][2];
extern const int S_SHAPE[2][4][2];
extern const int Z_SHAPE[2][4][2];