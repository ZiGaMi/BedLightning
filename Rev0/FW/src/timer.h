/*
 * timer.h
 *
 *  Created on: 22. dec. 2018
 *      Author: ZigaM
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f0xx.h"
#include "clock.h"
#include "stdlib.h"


/*
 * 	LEDs Driver -> PWM generation
 */
#define PWM_FREQ					( unsigned long )	( 25 ) // kHz
#define PWM_PERIOD					( uint16_t )		/*( 0x0781 )*/  ( SYS_CLK_FREQ / PWM_FREQ * 0.001f )
void LEDsDriverInit(void);


#define LED_FADE_TIMER_FREQ			( unsigned long )	( 100 ) // Hz
#define LED_FADE_TIMER_PRESCALER	( unsigned long )	( 4801u )
#define LED_FADE_TIMER_PERIOD		( unsigned long) 	( ( SYS_CLK_FREQ / LED_FADE_TIMER_PRESCALER ) / LED_FADE_TIMER_FREQ )
#define LED_FADE_TIMER_TIME_s		( float )			( 1.0f / ( float ) ( LED_FADE_TIMER_FREQ ))

/*
 * 	Rotary encoder
 */
// NOTE: 1kHz TOP limit
#define ENCODER_TIMER_FREQ			( unsigned long ) 	( 500 ) // Hz
#define ENCODER_TIMER_PRESCALER		( unsigned long )	( 4801u )
#define ENCODER_TIMER_PERIOD		( unsigned long) 	( ( SYS_CLK_FREQ / ENCODER_TIMER_PRESCALER ) / ENCODER_TIMER_FREQ )
#define ENCODER_TIMER_TIME_s		( float )			( 1.0f / ENCODER_TIMER_FREQ )

// Maximum encoder ticks
#define ENCODER_MAX_TICKS			( uint16_t )		( 200u )

void RotaryEncoderInit(void);




#endif /* TIMER_H_ */
