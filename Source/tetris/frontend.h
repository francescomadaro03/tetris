
#include <stdint.h>
#include "backend.h"

#ifndef __FRONTEND_TETRIS
#define __FRONTEND_TETRIS

extern volatile TETRONIM State;

extern void init_tetris_frontend(void);
extern void DrawSquare (uint16_t x0, uint16_t y0, uint16_t color);
extern char* ScoreToString(uint16_t score);
extern uint16_t LFSR_Random32(uint16_t state);
extern void DrawTetroid_I(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern void DrawTetroid_O(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern void DrawTetroid_T(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern void DrawTetroid_J(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern void DrawTetroid_L(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern void DrawTetroid_S(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern void DrawTetroid_Z(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation);
extern char RandomTetroidGenerator(void);
extern char FindColorFromCode(uint16_t color);
extern uint16_t Find_Color_From_Type(char typeT);
TETRONIM TetroidDrawer(uint16_t x0, uint16_t y0, uint16_t color, uint8_t rotation, char typeT);
extern void NewTetroid(void);
extern void ChangeDirection(TETRONIM State, char direction);
extern uint16_t FindColorCodeFromColor(char color);
extern void GameOver(void);
void ComputePoints(uint16_t CanceledRows, uint8_t SingleTetronim);
extern TETRONIM SpecialBlockDefinition(void);

#endif