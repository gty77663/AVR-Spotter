/*
 * Beeper.h
 *
 * Created: 01/06/2023 22:21:29
 *  Author: Kirill
 */ 

#pragma once

#include "../../../Config.h"

#ifdef BEEPER_SUPPORT

#include <string.h>
#include <avr/pgmspace.h>
#include "../../../Utils.h"
#include "../../../Consts.h"
#include "usart.h"

typedef struct {
	unsigned int duration;
	unsigned int frequency;
} tone_t;

typedef struct {
	tone_t tone;
	unsigned long endtime;
} state_t;

void Beeper_Tone(unsigned int duration, unsigned int frequency);
void Beeper_Tick(void);
void Beeper_Reset(void);
void Beeper_On(void);
void Beeper_Off(void);

#endif /* BEEPER SUPPORT */