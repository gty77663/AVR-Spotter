/*
 * LCD_SPI_12864.h
 *
 * Created: 30/05/2023 23:22:57
 *  Author: Kirill
 */ 

#pragma once

#ifndef LCD_SPI_12864_H_
#define LCD_SPI_12864_H_

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/sfr_defs.h> 
#include <util/atomic.h> // ATOMIC_BLOCK
#include <stdio.h> // sprintf()
#include <string.h> // strlen()
#include "u8g2.h"
#include "../../../Pins.h"
#include "../../../Consts.h" 
#include "../../../Utils.h" 
#include "usart.h"

#define P_CPU_NS (1000000000UL / F_CPU)

/* Public methods */
void LCD_Init(void);
int LCD_Update(display_data *data);

/* Private methods */
uint8_t u8x8_avr_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

u8g2_t u8g2;

#endif /* LCD_SPI_12864_H_ */