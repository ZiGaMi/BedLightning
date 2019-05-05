/*
 * gpio.h
 *
 *  Created on: 26.10.2018
 *      Author: ziga.miklosic
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f031x6.h"


/*
 *	LEDs
 */
#define LED_PORT		( GPIOA )
#define LED_T_bp		( 2ul )
#define LED_Z_bp		( 3ul )


/*
 * 	BUTTONs
 */
#define BUTTON_PORT		( GPIOB )
#define BUTTON_T_bp		( 0ul )
#define BUTTON_Z_bp		( 1ul )

/*
 * 	ROTARY ENCODERS
 */
#define RE_T_PORT		( GPIOB )
#define RE_T_A_bp		( 4ul )
#define RE_T_B_bp		( 5ul )
#define RE_Z_PORT		( GPIOA )
#define RE_Z_A_bp		( 8ul )
#define RE_Z_B_bp		( 9ul )



/*
 *  DEBUG PIN
 */
#define DEBUG_PORT		( GPIOA )
#define DEBUG_pb		( 0ul )


/*
 * 	GPIO Functions
 */
void GpioInit(void);
void GpioPinOn(GPIO_TypeDef*, uint8_t);
void GpioPinOff(GPIO_TypeDef*, uint8_t);
void GpioPinToggle(GPIO_TypeDef*, uint8_t);


#endif /* GPIO_H_ */
