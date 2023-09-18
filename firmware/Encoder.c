/*
 * Encoder.c
 *
 * Created: 03/06/2023 16:22:16
 *  Author: Kirill
 */ 
#include "Encoder.h"

encoder_out encoder;

/*---------------------------------------------------*\
|  Encoder routine								      |
|  https://github.com/PaulStoffregen/Encoder          |
\*---------------------------------------------------*/
ISR (PCINT1_vect)
{
	static uint8_t encState;
	static uint8_t oldState = 1;
	
	uint8_t s = encState & 0b11;
	if (READ_BIT(ENCODER_PIN, ENCODER_A_BIT)) s |= 0b100;
	if (READ_BIT(ENCODER_PIN, ENCODER_B_BIT)) s |= 0b1000;
	
	if (oldState == 0b1000 && s == 0b1110)
	{
		encoder.position++;
		encoder.flag = 1;
	}
	else if (oldState == 0b100 && s == 0b1101)
	{
		encoder.position--;
		encoder.flag = 1;
	}

	encState = (s >> 2);
	oldState = s;
	
	static uint32_t encTimer;
	static uint8_t last_button_state = 1;
	
	if (READ_BIT(ENCODER_PIN, ENCODER_SW_BIT) && last_button_state == 0)
	{
		uint16_t last_pres = millis() - encTimer;
		if (last_pres > 200)
		{
			encoder.clicks++;
			encoder.flag = 1;
		}
		encTimer = millis();
	}
	last_button_state = READ_BIT(PINC, PINC3);
}