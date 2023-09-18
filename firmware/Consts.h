/*
 * Consts.h
 *
 * Created: 31/05/2023 20:13:24
 *  Author: Kirill
 */ 

#pragma once

#ifndef CONSTS_H_
#define CONSTS_H_

#define LOW		0
#define HIGH	1

#define FIELD_FISRT_PERIOD_INDEX	0
#define FIELD_DELAY_PERIOD_INDEX	1
#define FIELD_SECOND_PERIOD_INDEX	2
#define FIELD_WELD_COUNT_INDEX		3

/* Max pointer value for both screens */
#define MAIN_SCREEN_POINTER_MAX		3  
#define CONFIG_SCREEN_POINTER_MAX	4

#define MAIN_SCREEN		0
#define CONFIG_SCREEN	1

#define MAIN_SCREEN_PARAMETER_STEP	10

#define MENU_ITEM_TYPE_PARAMETER		0
#define MENU_ITEM_TYPE_COUNTER			1
#define MENU_ITEM_TYPE_BOOL				2
#define MENU_ITEM_TYPE_BUTTON			3

#define MAIN_SCREEN_PERIOD1			0
#define MAIN_SCREEN_PAUSE			1
#define MAIN_SCREEN_PERIOD2			2
#define MAIN_SCREEN_CONFIG			3
#define MAIN_SCREEN_WELDS			4

#define CONFIG_SCREEN_BACK			0
#define CONFIG_SCREEN_DETECT_SD		1
#define CONFIG_SCREEN_SD_LOGGING	2
#define CONFIG_SCREEN_RST_WELDS		3
#define CONFIG_SCREEN_USB_CTRL		4

/* Struct definitions */
/*
typedef struct config_screen {
	uint8_t sd_log;
	uint8_t usb_control;
} config_screen;

typedef struct main_screen {
	uint16_t pulse1;
	uint16_t pause;
	uint16_t pulse2;
	uint16_t welds; 
} main_screen;
*/

typedef struct menu_item {
	const char name[32];
	const uint8_t type;
	const uint16_t default_value;
	uint16_t value;
} menu_item;

typedef struct display_data {
	menu_item main_screen_items[5];
	menu_item config_screen_items[5];
	int8_t pointer;
	uint8_t screen;
	uint8_t edit_parameter_flag;
} display_data;

typedef struct encoder_data {
	int16_t position;
	uint8_t clicks;
	uint8_t flag;
} encoder_data;

#endif /* CONSTS_H_ */