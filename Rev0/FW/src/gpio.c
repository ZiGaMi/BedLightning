/*
 * gpio.c
 *
 *  Created on: 26.10.2018
 *      Author: ziga.miklosic
 */

#include "gpio.h"

// Set pin high
void GpioPinOn(GPIO_TypeDef *gpio, uint8_t pin){
	gpio -> BSRR |= ( 0x01u << pin );
}


// Set pin low
void GpioPinOff(GPIO_TypeDef *gpio, uint8_t pin){
	gpio -> BSRR |= (( 0x01u << pin ) << 16u );
}

// Toggle pin
void GpioPinToggle(GPIO_TypeDef *gpio, uint8_t pin){
	if (( gpio -> IDR & ( 0x01u << pin )) == ( 0x01u << pin )){
		GpioPinOff(gpio, pin);
	}
	else{
		GpioPinOn(gpio, pin);
	}
}


// Initialize GPIO
void GpioInit(void){


	/*
	 * 	Enable clock for GPIOA, GPIOB, GPIOF
	 */
	RCC -> AHBENR |= ( RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN );


	/*
	 * 	Configure LEDs drivers
	 */

	// Set alternative function
	LED_PORT -> MODER &= ~(( 0x03u << ( 2ul * LED_T_bp )) |
						   ( 0x03u << ( 2ul * LED_Z_bp )));
	LED_PORT -> MODER |= (( 0x02u << ( 2ul * LED_T_bp )) |
						  ( 0x02u << ( 2ul * LED_Z_bp )));

	// Push-pull
	LED_PORT -> OTYPER &= ~((( 0x03u << LED_T_bp )) |
							 ( 0x03u << LED_Z_bp ));

	// High speed
	LED_PORT -> OSPEEDR |= (( 0x03u << ( 2ul * LED_T_bp )) |
						    ( 0x03u << ( 2ul * LED_Z_bp )));

	// No pullup/pulldown
	LED_PORT -> PUPDR &= ~((( 0x03u << LED_T_bp )) |
							( 0x03u << LED_Z_bp ));

	// Alternative functions -> TIM2 CH2/CH3 - AF2
	LED_PORT -> AFR[0] |= (( 0x02u << ( 4 * LED_T_bp )) |
						   ( 0x02u << ( 4 * LED_Z_bp )));



	/*
	 * 	Configure rotary encoder
	 */

	// Set alternative function
	RE_T_PORT -> MODER &= ~(( 0x03u << ( 2 * RE_T_A_bp )) |
							( 0x03u << ( 2 * RE_T_B_bp )));
	RE_T_PORT -> MODER |= (( 0x02u << ( 2 * RE_T_A_bp )) |
						   ( 0x02u << ( 2 * RE_T_B_bp )));

	RE_Z_PORT -> MODER &= ~(( 0x03u << ( 2 * RE_Z_A_bp )) |
							( 0x03u << ( 2 * RE_Z_B_bp )));
	RE_Z_PORT -> MODER |= (( 0x02u << ( 2 * RE_Z_A_bp )) |
						   ( 0x02u << ( 2 * RE_Z_B_bp )));

	// Pull up enable
	RE_T_PORT -> PUPDR |= (( 0x01u << ( 2 * RE_T_A_bp )) |
			   	   	   	   ( 0x01u << ( 2 * RE_T_B_bp )));

	RE_Z_PORT -> PUPDR |= (( 0x01u << ( 2 * RE_Z_A_bp )) |
			   	   	   	   ( 0x01u << ( 2 * RE_Z_B_bp )));


	// Alternative function
	// TIM3 CH1/CH2 - AF1
	RE_T_PORT -> AFR[0] |= (( 0x01u << ( 4 * RE_T_A_bp )) |
							( 0x01u << ( 4 * RE_T_B_bp )));

	// TIM1 CH1/CH2 - AF2
	RE_Z_PORT -> AFR[1] |= (( 0x02u << ( 4 * ( RE_Z_A_bp - 8ul ))) |
							( 0x02u << ( 4 * ( RE_Z_B_bp - 8ul ))));


	/*
	 * 	Configure button
	 */

	// Input mode
	BUTTON_PORT -> MODER &= ~(( 0x03u << ( 2ul * BUTTON_T_bp)) |
							  ( 0x03u << ( 2ul * BUTTON_Z_bp )));

	// High speed
	BUTTON_PORT -> OSPEEDR |= (( 0x03u << ( 2ul * BUTTON_T_bp )) |
						       ( 0x03u << ( 2ul * BUTTON_T_bp )));

	// Pull up enable
	BUTTON_PORT -> PUPDR |= (( 0x01u << ( 2ul * BUTTON_T_bp )) |
			   	   	   	   	 ( 0x01u << ( 2ul * BUTTON_Z_bp )));

	// System configure clock enable
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

	// External interrupts on PB0 and PB1
	SYSCFG -> EXTICR[0] |= (( 0x01u << ( 4ul * BUTTON_T_bp )) |
							( 0x01u << ( 4ul * BUTTON_Z_bp )));

	// Unmask button interrupt
	EXTI -> IMR |= (( 1ul << BUTTON_T_bp) | ( 1ul << BUTTON_Z_bp ));

	// Falling edge only
	EXTI -> RTSR &= ~(( 1ul << BUTTON_T_bp) | ( 1ul << BUTTON_Z_bp ));
//	EXTI -> RTSR |= (( 1ul << BUTTON_T_bp) | ( 1ul << BUTTON_Z_bp ));

//	EXTI -> FTSR &= ~(( 1ul << BUTTON_T_bp) | ( 1ul << BUTTON_Z_bp ));
	EXTI -> FTSR |= (( 1ul << BUTTON_T_bp) | ( 1ul << BUTTON_Z_bp ));

	// Clear pending interrupt
	EXTI -> PR |= (( 1ul << BUTTON_T_bp) | ( 1ul << BUTTON_Z_bp ));

	// Enable interrupt
	NVIC_EnableIRQ(EXTI0_1_IRQn);



	/*
	 * 	Configure debug pin
	 */

	DEBUG_PORT -> MODER &= ~( 0x03u << ( 2ul * DEBUG_pb ));
	DEBUG_PORT -> MODER |= ( 0x01u << ( 2ul * DEBUG_pb ));

}



