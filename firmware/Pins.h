/*
 * Pins.h
 *
 * Created: 29/05/2023 23:51:12
 *  Author: Kirill
 */

#pragma once

#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>
#include "Config.h"

/* Other peripherals */
#define ZD_PIN		PIND2
#define ZD_PORT		PORTD

#define PWM_PIN		PIND4
#define PWM_DIR		DDRD
#define PWM_PORT	PORTD

#define SWITCH_PIN	PIND3
#define SWITCH_PORT	PORTD

/* Encoder */
#define ENCODER_PORT   PORTC
#define ENCODER_PIN	   PINC
#define ENCODER_A_BIT  PCINT11
#define ENCODER_B_BIT  PCINT10
#define ENCODER_SW_BIT PCINT9

/* Beeper, can be connected or not, defined in Config.h */
#define BEEPER_PIN		PINC5
#define BEEPER_DIR		DDRC
#define BEEPER_PORT		PORTC

/* RepRap LCDs and I2C LCDs */
#if defined(REPRAP)
	#if defined(REPRAP_12864_LCD)
		#define SCK_LCD_PIN		PIND7
		#define SCK_LCD_DIR		DDRD
		#define SCK_LCD_PORT	PORTD
		
		#define MOSI_LCD_PIN	PIND6
		#define MOSI_LCD_DIR	DDRD
		#define MOSI_LCD_PORT	PORTD
		
		#define CS_LCD_PIN	PIND5
		#define CS_LCD_DIR	DDRD
		#define CS_LCD_PORT	PORTD
		
	#elif defined(REPRAP_2004_LCD)
		#define E_LCD_PIN  PIND6
		#define E_LCD_DIR  DDRD
		#define E_LCD_PORT PORTD
		
		#define RS_LCD_PIN	PIND5
		#define RS_LCD_DIR  DDRD
		#define RS_LCD_PORT PORTD
		
		#define DB4_LCD_PIN  PIND7
		#define DB4_LCD_DIR  DDRD
		#define DB4_LCD_PORT PORTD
		
		#define DB5_LCD_PIN	 PINB2
		#define DB5_LCD_DIR	 DDRB
		#define DB5_LCD_PORT PORTB
		
		#define DB6_LCD_PIN	 PINB0
		#define DB6_LCD_DIR	 DDRB
		#define DB6_LCD_PORT PORTB
		
		#define DB7_LCD_PIN	 PINB1
		#define DB7_LCD_DIR	 DDRB
		#define DB7_LCD_PORT PORTB
	#endif
	
#elif defined(I2C_DISPLAY)
	#define I2C_PORT PORTC
	#define SCL_PIN  PINC5
	#define SDA_PIN  PINC4
#endif

#endif /* PINS_H_ */