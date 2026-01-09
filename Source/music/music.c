#include "music.h"
#include "timer.h"
#include <stdint.h>
#include "LPC17xx.h"

extern volatile starting_game;
int melody[] = {
    // Battuta 1 (E)
    NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
    // Battuta 2 (Am)
    NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
    // Battuta 3 (E/G# - ma melodia senza alterazioni)
    NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
    // Battuta 4 (Am) - La pausa finale è accorpata all'ultimo A4
    NOTE_C5, NOTE_A4, NOTE_A4,

    // Battuta 5 (Dm)
    NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
    // Battuta 6 (C)
    NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
    // Battuta 7 (E/B)
    NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
    // Battuta 8 (Am) - La pausa finale è accorpata all'ultimo A4
    NOTE_C5, NOTE_A4, NOTE_A4
};

uint8_t noteDurations[] = {
    // Battuta 1: Tam(4), ti(2)-ti(2), Tam(4), ti(2)-ti(2)
    4, 2, 2, 4, 2, 2,

    // Battuta 2: Tam(4), ti(2)-ti(2), Tam(4), ti(2)-ti(2)
    4, 2, 2, 4, 2, 2,

    // Battuta 3: Tam(4), ti(2)-ti(2), Tam(4), Tam(4)
    4, 2, 2, 4, 4,

    // Battuta 4: Tam(4), Tam(4), Taaaaam(8 - nota+pausa)
    4, 4, 8,

    // Battuta 5: Taaam(6 - puntata), ti(2), Tam(4), ti(2)-ti(2)
    6, 2, 4, 2, 2,

    // Battuta 6: Taaam(6 - puntata), ti(2), Tam(4), ti(2)-ti(2)
    6, 2, 4, 2, 2,

    // Battuta 7: Tam(4), ti(2)-ti(2), Tam(4), Tam(4)
    4, 2, 2, 4, 4,

    // Battuta 8: Tam(4), Tam(4), Taaaaam(8 - nota+pausa)
    4, 4, 8
};

uint16_t ClockCyclesFromFrequency(int freq){
	uint16_t ClockCycles;
	ClockCycles = 25000000 / (freq * 45);
	return ClockCycles;


}

void InitControllingTimer(void){
	static uint32_t iterations = 0;
	volatile uint32_t TimerLength = BASE_LENGTH*noteDurations[iterations];
	disable_timer(2);
	reset_timer(2);
	if(starting_game == 1){
		init_timer(2, TimerLength);
	}
	else {
		LPC_TIM2->MR0 = TimerLength;
		LPC_TIM2->MCR = 3;
	}
	enable_timer(2); 
	iterations++;
	if(iterations > (sizeof(melody) / sizeof(melody[0]))){
		iterations = 0;
	}



}

void playNote(void){
	static uint32_t iterations = 0;
	volatile uint32_t ClockCyclesCount = ClockCyclesFromFrequency(melody[iterations]);
	disable_timer(1);
	reset_timer(1);
	if(starting_game == 1){
		init_timer(1, ClockCyclesCount);
	}
	else {
		LPC_TIM1->MR0 = ClockCyclesCount;
		LPC_TIM1->MCR = 3;
	}
	enable_timer(1);
	iterations++;
	if(iterations > (sizeof(noteDurations) / sizeof(noteDurations[0]))){
		iterations = 0;

	}



}