/*
#include "util.h"

#define F_CPU 16000000UL
#define ISR_CONTROLL       REGISTER_BIT(PORTD, 4)
#define ISR_DDR            REGISTER_BIT(DDRD, 4)
#define BUTTON_PIN         REGISTER_BIT(PORTD, 3)

volatile int weldingPeriods [3] = {4, 10, 4};
int weldingPower [2] = {19, 7};
int debounceDelay = 300;
volatile int tempWeldingPeriods [3], tempWeldingPower [3];
volatile int ISRItteration, weldingDelayCounter;
volatile long timer0Counter;


void InitTim0() {
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;

  OCR0A = 249; // 1000 Hz (16000000/((249+1)*64))
  TCCR0A |= (1 << WGM01); // CTC
  TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler 64
  TIMSK0 |= (1 << OCIE0A); // Output Compare Match A Interrupt Enable
}

void InitTim2() {
    TCCR2A = TCCR2B = TCNT2 = TIMSK2 = 0;
    TCCR2B |= (1 << CS21) | (1 << CS20); // 32 prescaler
}

int main(void) {
  cli();
  
  InitTim0();
  InitTim2();
  
  //----------I/O setup----------
  EICRA |= (1 << ISC11) | (1 << ISC01); // Falling interrupt on INT1 and INT0
  EIMSK |= (1 << INT1);  // Turn on INT1

  DDRD |= (1 << DDD4); // set PD4 as output

  DDRD &= ~(1 << DDD3); // ensure PD3 is input
  PORTD |= (1 << PORTD3); // PD3 input pull-up, if DDRD3 is set low

  DDRD |= (1 << DDD5);        // !PROTOTYPE! //   pinMode(PIN_SSR_POWER, OUTPUT);
  PORTD |= (1 << PORTD5);     // !PROTOTYPE! //   digitalWrite(PIN_SSR_POWER, HIGH);
  
  sei();
  while (1);
}

ISR (TIMER0_COMPA_vect) {
  timer0Counter++;
}

ISR (INT0_vect) {
  PORTD &= ~(1 << PORTD4);

  if (EIMSK & (1 << INT1)) {
    EIMSK &= ~(1 << INT1);
  }
  else {
    if (tempWeldingPeriods[ISRItteration] < 2) ISRItteration++;
    else tempWeldingPeriods[ISRItteration]--;

    if (ISRItteration > 2) {
      PORTD &= ~(1 << PORTD4);
      EIMSK &= ~(1 << INT0);
      EIMSK |= (1 << INT1);  // Turn on INT1
      ISRItteration = -1;
    }

    if (ISRItteration != 1 && ISRItteration > -1) {
        weldingDelayCounter = 20 - tempWeldingPower[ISRItteration];
        TCNT2 = 0;
        TIMSK2 |= (1 << TOIE2);
    }
  }
}

ISR (TIMER2_OVF_vect) {
  weldingDelayCounter--;
  TCNT2 = 0;
  if (weldingDelayCounter == 0) {
    TIMSK2 = TCNT2 = 0;
    PORTD |= (1 << PORTD4);
    _delay_us(20);
    PORTD &= ~(1 << PORTD4);
  }
}

ISR (INT1_vect) {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = timer0Counter;
  
  if (interruptTime - lastInterruptTime > debounceDelay) {
    
    ISRItteration = 0;
    
    tempWeldingPower[0] = weldingPower[0];
    tempWeldingPower[2] = weldingPower[1];
    memcpy(tempWeldingPeriods, weldingPeriods, sizeof(weldingPeriods));
    tempWeldingPeriods[0]++;
    
    EIMSK |= (1 << INT0); // Turn on INT1 (Zero detection INT)
  }
  lastInterruptTime = interruptTime;
}*/

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

volatile int weldingPeriods [3] = {4, 10, 4};
int weldingPower [2] = {19, 7};
const int debounceDelay = 300;
volatile int tempWeldingPeriods [3], tempWeldingPower [3];
volatile int weldingDelayCounter, ISRItteration;
volatile long timer0Counter;


static inline int is_odd(int x) { return x & 1; }

void InitTim0() {
  TCCR0A = TCCR0B = TCNT0 = 0;

  OCR0A = 249; // 1000 Hz 
  TCCR0A |= (1 << WGM01); // CTC
  TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler 64
  TIMSK0 |= (1 << OCIE0A); // Output Compare Match A Interrupt Enable
}

void InitTim2() {
  TCCR2A = TCCR2B = TCNT2 = TIMSK2 = 0;
  TCCR2B |= (1 << CS21) | (1 << CS20); // 32 prescaler
}

int main(void) {
  cli();
  
  InitTim0();
  InitTim2();
  
  //-------------------- I/O setup --------------------
  EICRA |= (1 << ISC11) | (1 << ISC01); // Falling interrupt on INT1 and INT0
  EIMSK |= (1 << INT1);  // Turn on INT1

  DDRD |= (1 << DDD4); // set PD4 as output

  DDRD &= ~(1 << DDD3); // ensure PD3 is input
  PORTD |= (1 << PORTD3); // PD3 input pull-up, if DDRD3 is set low

  DDRD |= (1 << DDD5);        // !PROTOTYPE! //   pinMode(PIN_SSR_POWER, OUTPUT);
  PORTD |= (1 << PORTD5);     // !PROTOTYPE! //   digitalWrite(PIN_SSR_POWER, HIGH);
  
  sei();
  while (1);
}

ISR (TIMER0_COMPA_vect) {
  timer0Counter++;
}

ISR (INT0_vect) {
  PORTD &= ~(1 << PORTD4);

  if (EIMSK & (1 << INT1)) {
    EIMSK &= ~(1 << INT1);
  }
  else {
    if (tempWeldingPeriods[ISRItteration] < 2) ISRItteration++;
    else tempWeldingPeriods[ISRItteration]--;

    if (ISRItteration > 2) {
      PORTD &= ~(1 << PORTD4);
      EIMSK &= ~(1 << INT0);
      EIMSK |= (1 << INT1);  // Turn on INT1
      ISRItteration = -1;
    }

    if (ISRItteration != 1 && ISRItteration > -1) {
      weldingDelayCounter = 20 - tempWeldingPower[ISRItteration];
      TCNT2 = 0;
      TIMSK2 |= (1 << TOIE2);
    }
  }
}

ISR (TIMER2_OVF_vect) {
  weldingDelayCounter--;
  TCNT2 = 0;
  if (weldingDelayCounter == 0) {
    TIMSK2 = TCNT2 = 0;
    PORTD |= (1 << PORTD4);
    _delay_us(20);
    PORTD &= ~(1 << PORTD4);
  }
}

ISR (INT1_vect) {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = timer0Counter;
  
  if (interruptTime - lastInterruptTime > debounceDelay) {
    
    ISRItteration = 0;
    
    tempWeldingPower[0] = weldingPower[0];
    tempWeldingPower[2] = weldingPower[1];
    memcpy(tempWeldingPeriods, weldingPeriods, sizeof(weldingPeriods));
    tempWeldingPeriods[0]++;
    
    EIMSK |= (1 << INT0); // Turn on INT0 (Zero detection INT)
  }
  lastInterruptTime = interruptTime;
}
