/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.0.1   2019-01-02

The MIT License (MIT)
Copyright (c) 2018 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f0xx.h"
#include <stdbool.h>

#include "clock.h"
#include "gpio.h"
#include "timer.h"

#include "BedLightning.h"
#include "RotaryEncoderDrv.h"


/*
 * 	Rotary encoders
 */
RotaryEncoderTypedef TinkaraRE 	= { .timer = TIM3 };
RotaryEncoderTypedef ZigaRE		= { .timer = TIM1 };

// Timer overflow flag
volatile bool RE_f = false;

/*
 * 	LED Strips
 */
LEDStripOwnerTypeDef TinkaraLedStrip = {

	.owner 			= Tinkara,
	.led_status 	= LED_OFF,
	.led_brigthness = BRIGHTNESS_min,
	.healt			= OK,
	.RE				= &TinkaraRE
};


LEDStripOwnerTypeDef ZigaLedStrip = {

	.owner 			= Ziga,
	.led_status 	= LED_OFF,
	.led_brigthness = BRIGHTNESS_min,
	.healt			= OK,
	.RE				= &ZigaRE
};

// Led fading timer overflow
volatile bool ledCheck_f = false;

// Change LED status
void ChangeLedStripStatus(LEDStripOwnerTypeDef*);

/*
 * 	BUTTON PRESSED FLAG
 */
volatile bool TinkaraButtonPressed_f 	= false;
volatile bool ZigaButtonPressed_f 		= false;
volatile uint8_t i = 0;


typedef enum{
	BUTTON_NOT_PRESSED 		= ( uint8_t ) ( 0x00u ),
	BUTTON_SHORT_PRESSED 	= ( uint8_t ) ( 0x01u ),
	BUTTON_LONG_PRESSED 	= ( uint8_t ) ( 0x02u )
}ButtonPressedEnumTypeDef;

#define BUTTON_LONG_PRESS_TIME_s		( float ) ( 0.50f )	// sec
#define BUTTON_SHORT_PRESS_TIME_s		( float ) ( 0.05f )	// sec





/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{

	// Initialize system
	SysClockInit();
	SysTickInit();
	GpioInit();
	LEDsDriverInit();
	RotaryEncoderInit();


	// Pressed button time
	float buttonT_pressed_time = 0.0f;
	float buttonZ_pressed_time = 0.0f;

	// Roommate timeout time
	float roommateTimeoutTimeT = 0.0f;
	float roommateTimeoutTimeZ = 0.0f;

	// Button status
	ButtonPressedEnumTypeDef buttonStatusT = BUTTON_NOT_PRESSED;
	ButtonPressedEnumTypeDef buttonStatusZ = BUTTON_NOT_PRESSED;


	while (1)
	{

		/*
		* 	Tinkaras actions
		*/
		switch( buttonStatusT ){

			case BUTTON_NOT_PRESSED:
				break;

			case BUTTON_SHORT_PRESSED:

				// Led status
				ChangeLedStripStatus( &TinkaraLedStrip );

				break;

			case BUTTON_LONG_PRESSED:

				// When LED is OFF after long press allow turn on led
				if ( TinkaraLedStrip.led_status == LED_OFF ){
					ChangeLedStripStatus( &TinkaraLedStrip );
				}

				// Enter roommate control if you have right
				else if ( !( TinkaraLedStrip.roommate_control )){
					ZigaLedStrip.roommate_control = true;
				}

				break;

		}

		// Force same status as roommate
		if ( ZigaLedStrip.roommate_control ){
			SetLedStripStatus( &ZigaLedStrip, TinkaraLedStrip.led_status);
		}


		/*
		* 	Zigas actions
		*/
		switch( buttonStatusZ ){

			case BUTTON_NOT_PRESSED:
				break;

			case BUTTON_SHORT_PRESSED:

				// Led status
				ChangeLedStripStatus( &ZigaLedStrip );

				break;

			case BUTTON_LONG_PRESSED:

				// When LED is OFF after long press allow turn on led
				if ( ZigaLedStrip.led_status == LED_OFF ){
					ChangeLedStripStatus( &ZigaLedStrip );
				}

				// Enter roommate control
				else if ( !( ZigaLedStrip.roommate_control )){
					TinkaraLedStrip.roommate_control = true;
				}

				break;
		}

		// Force same status as roommate in roommate control
		if ( TinkaraLedStrip.roommate_control ){
			SetLedStripStatus( &TinkaraLedStrip, ZigaLedStrip.led_status);
		}



		/*
		* 	Encoder checking
		*
		* 	Timer: 100Hz ( 10 ms )
		*/
		if ( RE_f ){

			// Clear flag
			RE_f = false;

			// Check encoders
			RotaryEncoderCheck( &TinkaraRE );
			RotaryEncoderCheck( &ZigaRE );

			// Get brightness
			//GetLedBrightness( &TinkaraLedStrip );
			//GetLedBrightness( &TinkaraLedStrip );

			// Check for control
			if ( !( TinkaraLedStrip.roommate_control ))		GetLedBrightness( &TinkaraLedStrip );
			else											SetLedBrightness( &TinkaraLedStrip, ZigaLedStrip.led_brigthness );

			// Check for control
			if ( !( ZigaLedStrip.roommate_control ))		GetLedBrightness( &ZigaLedStrip );
			else											SetLedBrightness( &ZigaLedStrip, TinkaraLedStrip.led_brigthness );



			// Check TINKARA button length press
			if ( !(( BUTTON_PORT -> IDR & ( 1ul << BUTTON_T_bp )) == ( 1ul << BUTTON_T_bp ))){
				buttonT_pressed_time += ( float ) ( ENCODER_TIMER_TIME_s );
			}

			// Check ZIGA button length press
			if ( !(( BUTTON_PORT -> IDR & ( 1ul << BUTTON_Z_bp )) == ( 1ul << BUTTON_Z_bp ))){
				buttonZ_pressed_time += ( float ) ( ENCODER_TIMER_TIME_s );
			}


			// Roommate control time
			if ( ZigaLedStrip.roommate_control && !( TinkaraLedStrip.RE->active )){

				// Check for timeout
				if ( roommateTimeoutTimeZ >= ROOMMATE_CNTR_NON_ACTIVE_TIME_s ){

					ZigaLedStrip.roommate_control = false;
					roommateTimeoutTimeZ = 0.0f;
				}

				else{
					roommateTimeoutTimeZ += ( float ) ( ENCODER_TIMER_TIME_s );
				}
			}
			else{
				roommateTimeoutTimeZ = 0.0f;
			}

			// Roommate control time
			if ( TinkaraLedStrip.roommate_control && !( ZigaLedStrip.RE->active )){

				// Check for timeout
				if ( roommateTimeoutTimeT >= ROOMMATE_CNTR_NON_ACTIVE_TIME_s ){

					TinkaraLedStrip.roommate_control = false;
					roommateTimeoutTimeT = 0.0f;
				}

				else{
					roommateTimeoutTimeT += ( float ) ( ENCODER_TIMER_TIME_s );
				}
			}
			else{
				roommateTimeoutTimeT = 0.0f;
			}

		}



		/*
		 * 	BUTTON LENGTH CHECK
		 */

		// TINKARA Button
		if ((( BUTTON_PORT -> IDR & ( 1ul << BUTTON_T_bp )) == ( 1ul << BUTTON_T_bp ))){

			// Check press lenght
			if ( buttonT_pressed_time >= BUTTON_LONG_PRESS_TIME_s ){
				buttonStatusT = BUTTON_LONG_PRESSED;
			}
			else if ( buttonT_pressed_time >= BUTTON_SHORT_PRESS_TIME_s ){
				buttonStatusT = BUTTON_SHORT_PRESSED;
			}
			else{
				buttonStatusT = BUTTON_NOT_PRESSED;
			}

			// Clear button pressed time
			buttonT_pressed_time = 0.0f;
		}

		// ZIGA Button
		if ((( BUTTON_PORT -> IDR & ( 1ul << BUTTON_Z_bp )) == ( 1ul << BUTTON_Z_bp ))){

			// Check press lenght
			if ( buttonZ_pressed_time >= BUTTON_LONG_PRESS_TIME_s ){
				buttonStatusZ = BUTTON_LONG_PRESSED;
			}
			else if ( buttonZ_pressed_time >= BUTTON_SHORT_PRESS_TIME_s ){
				buttonStatusZ = BUTTON_SHORT_PRESSED;
			}
			else{
				buttonStatusZ = BUTTON_NOT_PRESSED;
			}

			// Clear button pressed time
			buttonZ_pressed_time = 0.0f;
		}





	  /*
	   * 	Led update
	   *
	   * 	Timer: 100ms ( 10 Hz )
	   */
		if ( ledCheck_f ){

			// Clear flag
			ledCheck_f = false;

			// Roommate control check ***
			// Update led strip
			UpdateLedStrip( &TinkaraLedStrip );
			UpdateLedStrip( &ZigaLedStrip );
		}
	}
}




// Buttons ISR
void EXTI0_1_IRQHandler(void){

	// Set flag
	if ( !(( BUTTON_PORT -> IDR & ( 1ul << BUTTON_T_bp )) == ( 1ul << BUTTON_T_bp )) ){
		TinkaraButtonPressed_f = true;

	}

	// Set flag
	if ( !(( BUTTON_PORT -> IDR & ( 1ul << BUTTON_Z_bp )) == ( 1ul << BUTTON_Z_bp )) ){
		ZigaButtonPressed_f = true;
	}

	// Clear pending interrupt
	EXTI -> PR |= (( 1 << BUTTON_T_bp) | ( 1 << BUTTON_Z_bp ));
}



// Encoder checking ISR
void TIM14_IRQHandler(void){

	// Set RE flag
	RE_f = true;

	// Clear interrupt flag
	TIM14 -> SR &= ~ TIM_SR_UIF;
}


// Led check ISR
void TIM16_IRQHandler(void){

	// Set flag
	ledCheck_f = true;

	// Clear interrupt flag
	TIM16 -> SR &= ~ TIM_SR_UIF;
}

















