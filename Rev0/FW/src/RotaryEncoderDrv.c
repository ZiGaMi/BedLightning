/*
 * RotaryEncoder.c
 *
 *  Created on: 29. dec. 2018
 *      Author: ZigaM
 */

#include "RotaryEncoderDrv.h"

// Check rotary encoder
void RotaryEncoderCheck(RotaryEncoderTypedef *RE){

	// Get ticks
	RE -> cnt = RE -> timer -> CNT;

	// Calculate difference
	RE -> cnt_diff = ((( ENCODER_MAX_TICKS >> 1u ) - RE -> cnt ));

	// Refill counter
	RE -> timer -> CNT = ( ENCODER_MAX_TICKS >> 1u );


	/*
	 * 	ACTIVENESS CHECK
	 */

	// Inactive timer
	static float in_active_time = 0.0f;

	// Clear non active time
	if ( RE -> cnt_diff ){
		in_active_time = 0.0f;
	}

	// Increment till inactive
	else if ( RE -> active ){
		in_active_time += ENCODER_TIMER_TIME_s;
	}

	// Check for timer overflow
	if ( in_active_time >= ENCODER_NON_ACTIVE_TIME_s ){
		RE -> active = false;
	}
	else{
		RE -> active = true;
	}

}
