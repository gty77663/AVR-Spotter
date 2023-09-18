/*
 * Beeper.c
 *
 * Created: 01/06/2023 22:21:37
 *  Author: Kirill
 */ 

#include "Beeper.h"

#ifdef BEEPER_SUPPORT

/* A simple one item buffer is used to keep current frequency and duration */
state_t current_state = {0};

void Beeper_Tone(unsigned int duration, unsigned int frequency) 
{ 
	tone_t new_tone = {duration, frequency};
	current_state.tone = new_tone;
}
	
void Beeper_Tick()
{
	const unsigned long now = millis();
	
	if (!current_state.endtime)
	{
		if (current_state.tone.frequency > 0)
		{
			current_state.endtime = now + current_state.tone.duration;	
			
			#ifdef BEEPER_PWM
				/* Not implemented */
			#else
				Beeper_On();
			#endif
		}
	}
	long time_remaining = now - current_state.endtime; 
	if (time_remaining > 0)
	{
		Beeper_Reset();
	}
}

void Beeper_Reset(void)
{
	current_state = (const state_t){0};
	// memset(&current_state, 0, sizeof(state_t));
	Beeper_Off();
}

void Beeper_On(void) { SET_BIT(BEEPER_PORT, BEEPER_PIN); }
	
void Beeper_Off(void) { CLEAR_BIT(BEEPER_PORT, BEEPER_PIN); }

#endif /* BEEPER_SUPPORT */