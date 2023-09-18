/*
 * LCD_SPI_12864.c
 *
 * Created: 30/05/2023 23:22:48
 *  Author: Kirill
 */ 

#include "LCD_SPI_12864.h"

static char* Get_Item_String(char *str, menu_item *itm)
{
	if (itm->type == MENU_ITEM_TYPE_BOOL) 
	{
		(void)sprintf(str, "%s:%s", itm->name, Bool_To_Char(itm->value));
	}
	else if (itm->type == MENU_ITEM_TYPE_PARAMETER) 
	{
		(void)sprintf(str, "%s:%dmS", itm->name, itm->value);	
	} 
	
	return str;
}

u8g2_t u8g2;

void LCD_Init(void)
{
	u8g2_Setup_st7920_s_128x64_2(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_avr_gpio_and_delay);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
}

int LCD_Update(display_data *data)
{
	if (data->edit_parameter_flag)
	{
		menu_item *itm = Get_Current_Menu_Item(data);
		char str[24];
		(void)Get_Item_String(str, itm);
		uart_putstr(str);
		uart_puts_P("\r\n");
		
		u8g2_FirstPage(&u8g2);
		do {
			int x = 64-(strlen(str)*5)/2;
			u8g2_DrawStr(&u8g2, x, 35, str);
		} while (u8g2_NextPage(&u8g2));
		
		return 1;
	}
	
	switch (data->screen)
	{
		case MAIN_SCREEN:
			u8g2_FirstPage(&u8g2);
			do {
				/* P1, pause, P2 */
				for (int i = 0; i < 3; i++)
				{
					char str[16] = "";
					(void)Get_Item_String(str, &data->main_screen_items[i]);
					u8g2_DrawStr(&u8g2, 6, 7+i*10, str);
				}
				u8g2_DrawLine(&u8g2, 2, 29, 121, 29); /* Separation line */ 
				u8g2_DrawStr(&u8g2, 6, 38, data->main_screen_items[3].name); /* Config-> */ 
				/* WELDS:nnn */
				char str[16] = "";
				(void)Get_Item_String(str, &data->main_screen_items[4]);
				u8g2_DrawStr(&u8g2, 80, 7, str);
				/* Pointer > */
				u8g2_DrawStr(&u8g2, 0, 7+data->pointer*10, ">");
			} while (u8g2_NextPage(&u8g2));
			break;
			
		case CONFIG_SCREEN:
			u8g2_FirstPage(&u8g2);
			do {
				for (int i = 0; i < 5; i++)
				{
					if (data->config_screen_items[i].type == MENU_ITEM_TYPE_BUTTON)
					{
						u8g2_DrawStr(&u8g2, 6, 7+i*10, data->config_screen_items[i].name);
					}
					else if (data->config_screen_items[i].type == MENU_ITEM_TYPE_BOOL)
					{
						char str[24];
						sprintf(str, "%s:%s", data->config_screen_items[i].name, Bool_To_Char(data->config_screen_items[i].value));
						u8g2_DrawStr(&u8g2, 6, 7+i*10, str);
					}
				}	
				/* Pointer > */
				u8g2_DrawStr(&u8g2, 0, 7+data->pointer*10, ">");
			} while (u8g2_NextPage(&u8g2));
			break;
		default:
			break;	
	}
	
	return 1;
}

uint8_t u8x8_avr_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t cycles;

	switch(msg)
	{
		case U8X8_MSG_DELAY_NANO:     // delay arg_int * 1 nano second
		// At 20Mhz, each cycle is 50ns, the call itself is slower.
		break;
		case U8X8_MSG_DELAY_100NANO:    // delay arg_int * 100 nano seconds
		// Approximate best case values...
		#define CALL_CYCLES 26UL
		#define CALC_CYCLES 4UL
		#define RETURN_CYCLES 4UL
		#define CYCLES_PER_LOOP 4UL

		cycles = (100UL * arg_int) / (P_CPU_NS * CYCLES_PER_LOOP);

		if(cycles > CALL_CYCLES + RETURN_CYCLES + CALC_CYCLES)
		break;

		__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (cycles) : "0" (cycles) // 2 cycles
		);
		break;
		case U8X8_MSG_DELAY_10MICRO:    // delay arg_int * 10 micro seconds
		for(int i=0 ; i < arg_int ; i++)
		_delay_us(10);
		break;
		case U8X8_MSG_DELAY_MILLI:      // delay arg_int * 1 milli second
		for(int i=0 ; i < arg_int ; i++)
		_delay_ms(1);
		break;
		default:
		return 0;
	}
	return 1;
}


uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	// Re-use library for delays

	switch(msg)
	{
		case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
			SCK_LCD_DIR |= 1<<SCK_LCD_PIN;
			MOSI_LCD_DIR |= 1<<MOSI_LCD_PIN;
			CS_LCD_DIR |= 1<<CS_LCD_PIN;
			break;              // can be used to setup pins
		case U8X8_MSG_GPIO_SPI_CLOCK:        // Clock pin: Output level in arg_int
			if(arg_int)
			SCK_LCD_PORT |= (1<<SCK_LCD_PIN);
			else
			SCK_LCD_PORT &= ~(1<<SCK_LCD_PIN);
			break;
		case U8X8_MSG_GPIO_SPI_DATA:        // MOSI pin: Output level in arg_int
			if(arg_int)
				MOSI_LCD_PORT |= (1<<MOSI_LCD_PIN);
			else
				MOSI_LCD_PORT &= ~(1<<MOSI_LCD_PIN);
			break;
		case U8X8_MSG_GPIO_CS:        // CS (chip select) pin: Output level in arg_int
			if(arg_int)
				CS_LCD_PORT |= (1<<CS_LCD_PIN);
			else
				CS_LCD_PORT &= ~(1<<CS_LCD_PIN);
			break;
		default:
			if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))	// check for any delay msgs
				return 1;
			u8x8_SetGPIOResult(u8x8, 1);      // default return value
			break;
	}
	return 1;
}