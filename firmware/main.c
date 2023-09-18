/*
 * AVR-Spotter.c
 *
 * Created: 21/05/2023 16:44:36
 * Author : Kirill
 */ 

#include <avr/io.h>
#include <util/atomic.h> // ATOMIC_BLOCK
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h> // PROGMEM
#include "usart.h"
#include "Utils.h"
#include "Consts.h"
#include "Config.h"
#include "Beeper.h"

#if defined(REPRAP_12864_LCD)
	#include "LCD_SPI_12864.h"
#elif defined(REPRAP_2004_LCD)
	#include "LCD_SERIAL_2004.h"
#elif defined(I2C_2004_LCD)
	#include "LCD_I2C_2004.h"
#elif defined(I2C_16x2_LCD)
	#include "LCD_I2C_16x2.h"
#endif

encoder_data encoder = {};  // This object needs to be global, because encoder interrupt handler writes to it.
uint8_t LCD_Flag = 1;

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
		//if (last_pres > 200)
		{
			encoder.clicks++;
			encoder.flag = 1;
		}
		encTimer = millis();
	}
	last_button_state = READ_BIT(ENCODER_PIN, ENCODER_SW_BIT);
}

/**
 * Represents a book.
 * @constructor
 */
display_data* Encoder_Handler(display_data * data)
{
	menu_item *itm = Get_Current_Menu_Item(data);
	
	if (encoder.clicks)
	{
		Beeper_Tone(100, 100);
		
		/* Stop editing parameter */
		if (data->edit_parameter_flag) 
		{	
			data->edit_parameter_flag = 0;
			return data;
		}
		
		/* Enter parameter edit mode if it is editable */
		if (itm->type == MENU_ITEM_TYPE_BOOL || itm->type == MENU_ITEM_TYPE_PARAMETER) data->edit_parameter_flag = 1;
		
		/* Main screen buttons handler */
		else if (data->screen == MAIN_SCREEN)
		{
			/* Screen change */
			if (data->pointer == MAIN_SCREEN_CONFIG)
			{
				data->screen = CONFIG_SCREEN;
				data->pointer = 0;
			}
		}
		/* Config screen buttons handler */
		else if (data->screen == CONFIG_SCREEN)
		{
			switch (data->pointer)
			{
				case CONFIG_SCREEN_BACK:
					data->screen = MAIN_SCREEN;
					data->pointer = 0;
					break;
				case CONFIG_SCREEN_DETECT_SD:
					// TODO: add detect sd function call
					break;
				case CONFIG_SCREEN_RST_WELDS:
					memset(&data->main_screen_items[MAIN_SCREEN_WELDS], 0, sizeof(data->main_screen_items[MAIN_SCREEN_WELDS]));
					break;
				default:
					break;
			}
		}
	}
	if(encoder.position)
	{
		if (!data->edit_parameter_flag)
		{
			/* Move the pointer */
			data->pointer += encoder.position;
			if (data->screen == MAIN_SCREEN) data->pointer = CONSTRAIN(data->pointer, 0, MAIN_SCREEN_POINTER_MAX);
			else if (data->screen == CONFIG_SCREEN) data->pointer = CONSTRAIN(data->pointer, 0, CONFIG_SCREEN_POINTER_MAX);
			
			return data;
		}
		
		/* Edit parameter flag = 1 */ 
		if (data->screen == MAIN_SCREEN)
		{
			/* Edit welding parameters on main screen on encoder turn */
			itm->value += encoder.position * MAIN_SCREEN_PARAMETER_STEP;
			uart_putint(itm->value);
			itm->value = CONSTRAIN(itm->value, 10, 980);
		}
		else if (data->screen == CONFIG_SCREEN)
		{
			if (itm->type == MENU_ITEM_TYPE_BOOL) ((encoder.position < 0) ? (itm->value = 0) : (itm->value = 1));
		}
	}
	
	return data;
}

display_data Menu_Init() 
{
	display_data buffer; 
	menu_item main_sc[5] = {
		{"PERIOD1", MENU_ITEM_TYPE_PARAMETER, 10, 10},
		{"PAUSE", MENU_ITEM_TYPE_PARAMETER, 10, 10},
		{"PERIOD2", MENU_ITEM_TYPE_PARAMETER, 10, 10},
		{"CONFIG->", MENU_ITEM_TYPE_BUTTON},
		{"WELDS", MENU_ITEM_TYPE_COUNTER}
	};
	menu_item config_sc[5] = {
		{"BACK->", MENU_ITEM_TYPE_BUTTON},
		{"DETECT SD CARD", MENU_ITEM_TYPE_BUTTON},
		{"SD CARD LOGGING", MENU_ITEM_TYPE_BOOL},
		{"RESET WELD COUNTER", MENU_ITEM_TYPE_BUTTON},
		{"OVER-USB CONTROLL", MENU_ITEM_TYPE_BOOL}
	};
	
	memcpy(buffer.main_screen_items, main_sc, sizeof(main_sc));
	memcpy(buffer.config_screen_items, config_sc, sizeof(config_sc));
	
	return buffer;
}

static void Reset_System_Data(display_data *src)
{
	src->pointer = 0;
	src->screen = 0;
	src->edit_parameter_flag = 0;
}

int main(void)
{	
	cli();
	
	/* UART Init */
	uart0_init(BAUD_CALC(115200));
	stdout = &uart0_io; // attach UART stream to stdout & stdin
	stdin = &uart0_io;
	
	LCD_Init();
	Millis_Init();
	IO_Init();
	
	sei();
	
	display_data data = Menu_Init();
	
	/* If external/power-on reset occurs - reset data structure */
	if (READ_BIT(MCUSR, EXTRF) || READ_BIT(MCUSR, PORF)) 
	{
		Reset_System_Data(&data);	
	}
		
	uart_puts("t");
	
    while (1) 
    {
		/* Encoder handler */
		if (encoder.flag)
		{
			(void)Encoder_Handler(&data);
			LCD_Flag = 1;
			encoder = (const encoder_data){0};
		}
		/* Update the LCD */
		if (LCD_Flag)
		{
			(void)LCD_Update(&data);
			LCD_Flag = 0;
		}
		Beeper_Tick();
    }
}

