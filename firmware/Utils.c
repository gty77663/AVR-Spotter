/*
 * Utils.c
 *
 * Created: 01/06/2023 23:02:47
 *  Author: Kirill
 */ 

#include "Utils.h"

unsigned long millis_counter = 0; 

void Millis_Init()
{
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	
	OCR0A = 249;
	
	TIMSK0 = _BV(OCIE0A);
}

unsigned long millis() { return millis_counter;}

ISR(TIMER0_COMPA_vect)
{
	millis_counter++;
}

void IO_Init(void)
{
	/* Encoder internal pull-up */
	ENCODER_PORT |= _BV(ENCODER_SW_BIT) | _BV(ENCODER_B_BIT) | _BV(ENCODER_A_BIT); 
	/* Encoder pin-change interrupts */
	PCMSK1 |= _BV(ENCODER_SW_BIT) | _BV(ENCODER_B_BIT) | _BV(ENCODER_A_BIT);
	PCICR |= _BV(PCIE1);
	
	/* Other peripherals */
	WRITE_BIT(ZD_PORT, ZD_PIN, HIGH);
	WRITE_BIT(PWM_DIR, PWM_PIN, HIGH);
	WRITE_BIT(SWITCH_PORT, SWITCH_PIN, HIGH);
	WRITE_BIT(BEEPER_DIR, BEEPER_PIN, HIGH);
}

menu_item* Get_Current_Menu_Item(display_data *data)
{
	if (data->screen == MAIN_SCREEN)
	{
		return &data->main_screen_items[data->pointer];
	}
	else if (data->screen == CONFIG_SCREEN)
	{
		return &data->config_screen_items[data->pointer];
	}
	
	return;
}

char* Bool_To_Char(uint8_t bool)
{
	char *str = "";
	if (bool) str = "True";
	else str = "False";
	
	return str;
}