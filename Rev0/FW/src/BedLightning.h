/*
 * BedLightning.h
 *
 *  Created on: 2. jan. 2019
 *      Author: ZigaM
 */

#ifndef BEDLIGHTNING_H_
#define BEDLIGHTNING_H_


#include "stm32f0xx.h"
#include <stdbool.h>
#include <math.h>

#include "timer.h"
#include "RotaryEncoderDrv.h"


/*
 * 	Owners
 */
typedef enum{
	Tinkara = ( uint8_t ) ( 0x01u ),
	Ziga 	= ( uint8_t ) ( 0x02u )
}OwnerEnumTypeDef;


/*
 * 	Healt
 */
typedef enum{
	OK		= ( uint8_t ) ( 0x00u ),
	OVC		= ( uint8_t ) ( 0x01u )		// Over current
}HealtEnumTypeDef;


/*
 * 	LED Status
 */
typedef enum{

	LED_OFF 		= ( uint8_t ) ( 0x00u ),
	LED_ON			= ( uint8_t ) ( 0x01u ),
	LED_FADE_IN		= ( uint8_t ) ( 0x02u ),
	LED_FADE_OUT	= ( uint8_t ) ( 0x03u)
}LedStatusEnumTypeDef;


/*
 * 	LED Strip Owner
 */
typedef struct{

	// Owner of LED strip
	OwnerEnumTypeDef owner;

	// Roommate control
	bool roommate_control;

	// LED status
	LedStatusEnumTypeDef led_status;

	// LED brightness
	float led_brigthness;

	// LED fade out time
	float led_fade_time;

	// Health
	HealtEnumTypeDef healt;

	// Rotary encoder
	RotaryEncoderTypedef *RE;

}LEDStripOwnerTypeDef;


/*
 * 	LEDs defines
 */
#define BRIGHTNESS_min 			( float ) ( 0.10f )
#define BRIGHTNESS_max			( float ) ( 1.00f )
#define BRIGHTNESS_res			( float ) ( 0.10f )


#define LED_FADE_grad			( float ) ( 0.025f )											// 2.5 % / 10ms
#define LED_FADE_TIME_s			( float ) (( 1.0f / LED_FADE_grad ) * LED_FADE_TIMER_TIME_s )	// sec


/*
 * 	ROOMMATE CONTROL defines
 */
#define ROOMMATE_CNTR_NON_ACTIVE_TIME_s		( float )	( 2.0f )	// sec



/*
 * 	LED functions
 */
void UpdateLedStrip(LEDStripOwnerTypeDef*);
void GetLedBrightness(LEDStripOwnerTypeDef*);
void SetLedBrightness(LEDStripOwnerTypeDef*, float);
void CheckFadeTime(LEDStripOwnerTypeDef*);
void ChangeLedStripStatus(LEDStripOwnerTypeDef*);
void SetLedStripStatus(LEDStripOwnerTypeDef*, LedStatusEnumTypeDef);

#endif /* BEDLIGHTNING_H_ */
