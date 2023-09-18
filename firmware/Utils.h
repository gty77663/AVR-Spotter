/*
 * Utils.h
 *
 * Created: 01/06/2023 23:02:56
 *  Author: Kirill
 */ 

#pragma once

#ifndef UTILS_H_
#define UTILS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

#include "Config.h"
#include "Pins.h"
#include "Consts.h"

#define CONSTRAIN(amt,low,high)		((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define READ_BIT(sfr, bit)			(((sfr) >> (bit)) & 0x01)
#define WRITE_BIT(reg, bit, state) if (state) reg |= _BV(bit); else reg &= ~(_BV(bit));
#define CLEAR_BIT(sfr, bit)			(_SFR_BYTE(sfr) &= ~_BV(bit)) 
#define SET_BIT(sfr, bit)			(_SFR_BYTE(sfr) |= _BV(bit))  

void Millis_Init();
unsigned long millis();
void IO_Init(void);
menu_item* Get_Current_Menu_Item(display_data *data);
char* Bool_To_Char(uint8_t bool);

#endif /* UTILS_H_ */