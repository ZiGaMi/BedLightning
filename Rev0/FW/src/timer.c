/*
 * timer.c
 *
 *  Created on: 22. dec. 2018
 *      Author: ZigaM
 */

#include "timer.h"



// Initialization of current reference - TIM2 CH3 and CH4
void LEDsDriverInit(){

	// Enable timer clock
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Disable timer
	TIM2 -> CR1 = 0x0000u;

	// Output compare mode -> PWM Mode 1 for channel 2
	//TIM2 -> CCMR1 |= ( 0x06u << TIM_CCMR1_OC2M_Pos );

	// Output compare mode -> PWM Mode 1 for CH3/CH4
	TIM2 -> CCMR2 |= (( 0x06u << TIM_CCMR2_OC3M_Pos ) |
					  ( 0x06u << TIM_CCMR2_OC4M_Pos ));

	// Timer clock prescaler
	TIM2 -> PSC = 0u;

	// Timer frequency
	TIM2 -> ARR = ( uint32_t ) ( PWM_PERIOD );

	// Enable TIM2 channel 3 and 4 output
	TIM2 -> CCER |= ( TIM_CCER_CC3E | TIM_CCER_CC4E );

	// Enable main output
	TIM2 -> BDTR |= TIM_BDTR_MOE;

	// Compare registers
	TIM2 -> CCR3 = 0u;
	TIM2 -> CCR4 = 0u;

	// Enable timer
	TIM2 -> CR1 |= TIM_CR1_CEN;



	/*
	 * 	Configure timer16 for LED fading
	 */

	// Enable timer 16
	RCC -> APB2ENR |= RCC_APB2ENR_TIM16EN;

	// Disable timer
	TIM16 -> CR1 = 0x0000u;

	// Update interrupt enable
	TIM16 -> DIER |= TIM_DIER_UIE;

	// Prescaler
	TIM16 -> PSC = ( uint16_t ) ( LED_FADE_TIMER_PRESCALER );

	// Period
	TIM16 -> ARR = ( uint16_t ) ( LED_FADE_TIMER_PERIOD );

	// Clear counter
	TIM16 -> CNT = 0ul;

	// Clear interrupt flag
	TIM16 -> SR &= ~TIM_SR_UIF;

	// Enable interrupt
	NVIC_EnableIRQ(TIM16_IRQn);

	// Enable timer
	TIM16 -> CR1 |= TIM_CR1_CEN;

}

// Initialization of rotay encoder - TIM1 CH1/CH2 and TIM3 CH1/CH2
void RotaryEncoderInit(void){

	/*
	 * 	Configure timer for encoder interface
	 */

	// Enable timer 1 and timer 3 clock
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;


	/*
	 * 	RE Z
	 */

	// Disable counter
	TIM1 -> CR1 = 0x0000u;

	// Mapped input channels to TI1
	TIM1 -> CCMR1 |= ( TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0 );

	// Non inverting input
	TIM1 -> CCER &= ~( TIM_CCER_CC1P | TIM_CCER_CC2P );

	// Encoder mode 3 -> both edges
	TIM1 -> SMCR |= ( TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1 );

	// Prescaler
	TIM1 -> PSC = 2ul;

	// Period
	TIM1 -> ARR = ENCODER_MAX_TICKS;

	// Clear counter
	TIM1 -> CNT = ( ENCODER_MAX_TICKS >> 1 );

	// Enable timer 1
	TIM1 -> CR1 |= TIM_CR1_CEN;


	/*
	 * 	RE T
	 */

	// Disable counter
	TIM3 -> CR1 = 0x0000u;

	// Mapped input channels to TI1
	TIM3 -> CCMR1 |= ( TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0 );

	// Non inverting input
	TIM3 -> CCER &= ~( TIM_CCER_CC1P | TIM_CCER_CC2P );

	// Encoder mode 3 -> both edges
	TIM3 -> SMCR |= ( TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1 );

	// Prescaler
	TIM3 -> PSC = 2ul;

	// Period
	TIM3 -> ARR = ENCODER_MAX_TICKS;

	// Clear counter
	TIM3 -> CNT = ( ENCODER_MAX_TICKS >> 1 );

	// Enable timer 1
	TIM3 -> CR1 |= TIM_CR1_CEN;



	/*
	 * 	Configure timer for encoder checking
	 */

	// Enable timer 14
	RCC -> APB1ENR |= RCC_APB1ENR_TIM14EN;

	// Disable timer
	TIM14 -> CR1 = 0x0000u;

	// Update interrupt enable
	TIM14 -> DIER |= TIM_DIER_UIE;

	// Prescaler
	TIM14 -> PSC = ( uint16_t ) ( ENCODER_TIMER_PRESCALER );

	// Period
	TIM14 -> ARR = ( uint16_t ) ( ENCODER_TIMER_PERIOD );

	// Clear counter
	TIM14 -> CNT = 0ul;

	// Clear interrupt flag
	TIM14 -> SR &= ~TIM_SR_UIF;

	// Enable interrupt
	NVIC_EnableIRQ(TIM14_IRQn);

	// Enable timer
	TIM14 -> CR1 |= TIM_CR1_CEN;
}














