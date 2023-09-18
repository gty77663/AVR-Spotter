/*
 * Encoder.h
 *
 * Created: 03/06/2023 16:22:25
 *  Author: Kirill
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#include <avr/interrupt.h>
#include "Utils.h"
#include "Pins.h"

typedef struct {
	uint8_t position;
	uint8_t clicks;	
	uint8_t flag;
} encoder_out;

extern encoder_out encoder;

#endif /* ENCODER_H_ */