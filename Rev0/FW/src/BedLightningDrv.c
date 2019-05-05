/*
 * BedLightningDrv.c
 *
 *  Created on: 2. jan. 2019
 *      Author: ZigaM
 */


#include "BedLightning.h"


// Set LED Strip
void UpdateLedStrip(LEDStripOwnerTypeDef *LS){

	// Duty cycle
	uint16_t duty = 0u;

	// LED Status
	switch ( LS -> led_status ){

		case LED_OFF:

			// Resets value
			duty = 0u;
			LS -> led_brigthness = BRIGHTNESS_min;

			// Roommate control off
			LS -> roommate_control = false;
			break;

		case LED_ON:

			// Calculate duty
			duty = (( LS -> led_brigthness * LS -> led_brigthness + BRIGHTNESS_min ) * PWM_PERIOD );
			break;

		case LED_FADE_IN:
			break;

		case LED_FADE_OUT:

			// Check time
			CheckFadeTime( LS );

			// Calculate duty
			duty = (( LS -> led_brigthness * LS -> led_brigthness ) * PWM_PERIOD );

			// Fade out effect
			if ( LS -> led_brigthness <= 0.0f ){
				LS -> led_brigthness = 0.0f;
			}
			else{
				LS -> led_brigthness -= LED_FADE_grad;
			}

			break;

		default:
			break;
	}

	// Set duty
	if ( LS -> owner == Tinkara )		TIM2 -> CCR3 = ( uint16_t ) ( duty );
	else if ( LS -> owner == Ziga )		TIM2 -> CCR4 = ( uint16_t ) ( duty );
}


// Get Led brightness
void GetLedBrightness(LEDStripOwnerTypeDef *LS){

	if ( LS -> led_status != LED_FADE_OUT ){

		// Get brightness
		LS -> led_brigthness -= ( float ) (( LS -> RE -> cnt_diff ) * ( BRIGHTNESS_res / 2.0f ));

		// Bound brightness
		if (( LS -> led_brigthness ) <= BRIGHTNESS_min ){
			LS -> led_brigthness = BRIGHTNESS_min;
		}
		else if (( LS -> led_brigthness ) >= BRIGHTNESS_max ){
			LS -> led_brigthness = BRIGHTNESS_max;
		}
	}
}

// Set LED brightness
void SetLedBrightness(LEDStripOwnerTypeDef *LS, float brigthness){
	LS -> led_brigthness = brigthness;
}


// Check fade time
void CheckFadeTime(LEDStripOwnerTypeDef *LS){

	// Check for fade out timeout
	if ( LS -> led_fade_time >= LED_FADE_TIME_s ){

	  // Clear fade timer
	  LS -> led_fade_time = 0.0f;

	  // Clear led turn off flag
	  LS -> led_status = LED_OFF;
	}

	// Increment timer
	else{
	  LS -> led_fade_time += ( float ) ( LED_FADE_TIMER_TIME_s );
	}
}


// Change LED status
void ChangeLedStripStatus(LEDStripOwnerTypeDef *LS){

	// Change LED Status
	switch( LS -> led_status ){

	  case LED_OFF:
		  LS -> led_status = LED_ON;
		  break;

	  case LED_ON:
		  LS -> led_status = LED_FADE_OUT;
		  break;

	  case LED_FADE_IN:
		  break;

	  case LED_FADE_OUT:
			 break;

	  default:
		  break;
	}
}

// Set LED strip status
void SetLedStripStatus(LEDStripOwnerTypeDef *LS, LedStatusEnumTypeDef status){
	LS -> led_status = status;
}
