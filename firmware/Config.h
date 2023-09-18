/*
 * Config.h
 *
 * Created: 29/05/2023 23:20:34
 *  Author: Kirill
 */ 

#pragma once

#ifndef CONFIG_H_
#define CONFIG_H_

/* LCD Config */
#define REPRAP_12864_LCD 1
// #define REPRAP_2004_LCD  1
// #define I2C_2004_LCD     1
// #define I2C_16x2_LCD     1

/* If beeper is connected separately from RepRap display it can be manually turned on */
// #define BEEPER_SUPPORT	1

#if defined(REPRAP_12864_LCD) || defined(REPRAP_2004_LCD)
	#define REPRAP			1
	#define SD_SUPPORT		1	
	#ifndef BEEPER_SUPPORT
		#define BEEPER_SUPPORT	1
	#endif
#endif

#if defined(I2C_2004_LCD) || defined(I2C_16x2_LCD)
	#define I2C_DISPLAY		1
#endif

/* UART BAUD */
#define BAUD		115200
#define MYUBRR		F_CPU/16/BAUD-1

#endif /* CONFIG_H_ */