#include "music.h"
#include "timer.h"
#include <stdint.h>
#include "LPC17xx.h"

extern volatile starting_game;
extern uint8_t ClearedLineFlag;
int tetris_melody[] = {
    NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
    NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
    NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_C5, NOTE_A4, NOTE_A4, 0,
	
    NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
    NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
    NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_C5, NOTE_A4, NOTE_A4, 0
};

uint8_t noteDurations1[] = {
  2, 1, 1, 1, 1, 1,
  1, 1, 2, 1, 1, 2,
  1, 1, 2, 1, 1,
  2, 2, 2, 4,

  1, 2, 1, 1, 2,
  2, 1, 2, 1, 2,
  1, 1, 2, 1, 1,
  2, 2, 2, 4
};


int clearedLineSounds[] = {
    NOTE_E6, NOTE_F6, NOTE_DS6
};

uint8_t noteDurations2[] = {
  1,1,1
};

uint32_t ClockCycleFromFrequency(int frequency) {
	uint32_t ClockCycles;
	ClockCycles = 25000000 / (frequency * 45);
	return ClockCycles;
}

void InitMusicTimer(void) {
	static volatile uint8_t NoteDurationIndex = 0;
	uint8_t volatile NoteDuration = noteDurations1[NoteDurationIndex];
	uint32_t volatile EffectiveDuration = BASE_LENGTH*NoteDuration;
	disable_timer(2);
	reset_timer(2);
	init_timer(2, EffectiveDuration);
	enable_timer(2);
	NoteDurationIndex++;
	if(NoteDurationIndex == 40) {
		NoteDurationIndex = 0;
	}
} 

void InitClearedLine(void) {
	static volatile uint8_t NoteDurationIndexC = 0;
	uint8_t volatile NoteDuration = noteDurations2[NoteDurationIndexC];
	uint32_t volatile EffectiveDuration = BASE_LENGTH*NoteDuration;
	disable_timer(2);
	reset_timer(2);
	init_timer(2, EffectiveDuration);
	enable_timer(2);
	NoteDurationIndexC++;
	if(NoteDurationIndexC == 3) {
		NoteDurationIndexC = 0;
	}
} 

void playNote(void) {
	static volatile uint16_t NoteIndex = 0;
	if (tetris_melody[NoteIndex] != 0) {
		volatile uint32_t ClockCycles = ClockCycleFromFrequency(tetris_melody[NoteIndex]);
		disable_timer(1);
		reset_timer(1);
		init_timer(1, ClockCycles);
		enable_timer(1);
	}
	else {
		disable_timer(1);
	}
	NoteIndex++;
	if(NoteIndex == 40) {
		NoteIndex = 0;
	}
}


void playNoteClearedLine(void) {
	static volatile uint16_t NoteIndex = 0;
	if (tetris_melody[NoteIndex] != 0) {
		volatile uint32_t ClockCycles = ClockCycleFromFrequency(clearedLineSounds[NoteIndex]);
		disable_timer(1);
		reset_timer(1);
		init_timer(1, ClockCycles);
		enable_timer(1);
	}
	else {
		disable_timer(1);
	}
	NoteIndex++;
	if(NoteIndex == 3) {
		ClearedLineFlag = 0;
		NoteIndex = 0;
	}
}