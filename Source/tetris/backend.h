
#include <stdint.h>
#define ROWS 21
#define COLS 10

extern char GAMESTATE[ROWS][COLS];


#ifndef TETROID_STRUCTS
#define TETROID_STRUCTS
	typedef struct {
		char TETRONIM_TYPE;
		char COLOR;
		uint16_t COLOR_CODE;
		int ROTATE;
		uint16_t COORD[2];
		uint16_t FLOOR_COORD[8]; //first two pairs are coordinates of the no rotation, then the rotation
		uint8_t HORIZONTAL_LENGTH; // number of blocks from upper coordinates
		uint8_t HORIZONTAL_ROTATE_LENGTH;
		uint8_t VERTICAL_LENGTH;
		char SPECIAL_BLOCK;

	
	} TETRONIM;
#endif





#ifndef __BACKEND_TETRIS
#define __BACKEND_TETRIS

extern void ConfigurationProcedureTiming(void);
extern void MovementInit(void);
extern TETRONIM VerticalMovementHandler(TETRONIM t);
extern TETRONIM HorizontalMovementHandler(TETRONIM t, char direction);
extern uint8_t CheckCollisions(TETRONIM State);
extern TETRONIM RotateCurrentTetronim(TETRONIM t);
extern uint8_t CheckFullRow(uint8_t row);
void CheckGAMESTATE(uint8_t row);
void VoidField(uint8_t row, uint8_t LastIndexFull);
void RedrawField(uint8_t start, uint8_t LastIndexFull);
void UpdateGAMESTATE(int8_t row, int8_t FirstIndexFull, int8_t LastIndexFull);	
void SpeedUpTimer(uint8_t flag, uint32_t speed);
void HardDropTetroid(TETRONIM State);
extern void Reset_GAMESTATE(void);
extern uint8_t RandomMalus(uint8_t Highest_Y);
extern void HandleTimerSpeed(uint8_t PotSpeed);
extern void ClearHalfField(void);
extern void PowerUpsManagement(void);
#endif