#include <stdint.h>

#define RCC_BASE  0x40021000 // The base address of the Reset and Clock Control register
#define TIM1_BASE 0x40012C00 // The base address of the Timer 1 Peripheral's register
#define USART2_BASE 0x40004400 // This is the base address of UART2 Register from the RM0008 documentation
#define NVIC_BASE 0xE000E100 // The base address for the Nested Vectored Interrupt Controller in the Arm Cortex M Core
#define GPIOA_BASE 0x40010800 // The base address of the GPIO Port A Peripheral Register

#define RCC_APB1ENR *(unsigned int *) (RCC_BASE + 0x1C) // The APB1ENR register has an offset of 0x1C
#define RCC_AP2ENR *(volatile unsigned int *) (RCC_BASE + 0x18) // The address of the RCC_APB2ENR Register

#define TIM1_PSC *(volatile unsigned int *) (TIM1_BASE + 0x28) // The address of the Timer 1 Prescaler Register
#define TIM1_ARR *(volatile unsigned int *) (TIM1_BASE + 0x2C) // The address of the Timer 1 Auto Reset Register
#define TIM1_DIER *(volatile unsigned int *) (TIM1_BASE + 0x0C) // The address of the TImer 1 Direct Memory Access Interrupt Enable Register
#define TIM1_SR *(volatile unsigned int *) (TIM1_BASE + 0x10) // The address of the Timer 1 Status Register
#define TIM1_CR1 *(volatile unsigned int *) (TIM1_BASE) // The address of the Timer 1 Control Register 1
#define NVIC_ISER0 *(volatile unsigned int *) (NVIC_BASE) // The address of the Nested Vectored Controller - Interrupt Set Enable Register 0

#define GPIOA_CRL *(unsigned int *) (GPIOA_BASE ) // The address of the GPIO Port A Configuration Register Low
#define GPIOA_BSRR *(unsigned int *) (GPIOA_BASE + 0x10) // The address of the GPIO Port A Bit Set and Reset Register

#define USART2_BRR (*(unsigned int *) (USART2_BASE + 0x08)) // The UART2 Baud Rate Register(BRR) has an offset of 0x08
#define USART2_CR1 (*(unsigned int *) (USART2_BASE + 0x0C)) // The UART2 Control 1 (CR1) register has an offset of 0x0C
#define USART2_SR (*(unsigned int *) (USART2_BASE)) // The UART2 Status Register is at the base of UART2 Register
#define USART2_DR (*(unsigned int *) (USART2_BASE + 0x04)) // The UART2 Data register has an offset of 0x04

#define FCLK 8000000 // The default clock speed for STM32 at startup is 8Mhz
#define BAUDRATE 115200 // The desired baud rate for USART communication is 115200

#define USARTDIV  FCLK/(16*BAUDRATE) // USART DIV is the value that guides the controller how to split the clock to achieve the desired baud rate
// The USARTDIV comes out to be 4.34. in hex 0x4 is 4 and 0x5 which is 0.34 * 16 = 5.44 for scaling and rounded to 5. The final result to be added to BRR becomes 0x45.

volatile int i = 0; // Counter for the toggle LED operation

int main(void) {
	RCC_AP2ENR |= (1 << 11); // Enable the Timer 1 Clock with Bit 11
	RCC_AP2ENR |= (1 << 2); // Enable the GPIO Port A Clock with Bit 2
	RCC_APB1ENR |= (1 << 17); // Enabling APB1 Bus in RCC for UART2

	GPIOA_CRL &= ~(0xF << 20); // Clear the Bits for PA5
	GPIOA_CRL |= (0x2 << 20); // Set pin PA5 for output at 2Mhz (0010)

	GPIOA_CRL &= ~(0xf << 8); // Clearing the bits for PA2, Bit 8-12
	GPIOA_CRL |= (0xb << 8); // Configuring PA2 to be a Alternate Function Output at 50MHz. (1011).

	USART2_BRR = (0x45); // Configuring the baud rate register for 115200 using the UARTDIV value (0x45)
	USART2_CR1 |= (1 << 13); // Enabling the UART Enabling bit (Bit 13) in the UART Control Register
	USART2_CR1 |= (1 << 3); // Enabling the Transmission Enable (Bit 3) in the UART Control Register

	TIM1_PSC = (799); // The prescaler is set to 800, therefore 8MHZ clock divided by 800 is 10Khz and a timer period of 0.1 ms
	TIM1_ARR = (9999); // The Auto Reset Register is set to 9999 according to off-by-one so that the timer interrupts after 0.1 ms * 10000 = 1 second.
	TIM1_DIER |= (1); // The Update Interrupt Enable bit of the Timer Direct Memory Access Interupt Enable Register (Bit 1) is enabled to allow timer to interrupt.

	NVIC_ISER0 |= (1 << 25); // The Bit 25, that is the interrupt queue position for Timer 1 Updates is set to 1 to enable NVIC to listen to timer interrupts

	TIM1_CR1 |= (1 << 0); // The counter enable bit of the Timer 1 Control Register which is bit 0 is enabled to start counter.

	while (1)
		; // Loop forever

}
void transmit_char(char chr) {
	while (!(USART2_SR & (1 << 7)))
		; // Waiting for the Bit 7 of the Status register to be 1, to signal that new data is ready to be received.
	USART2_DR = chr; // Writing the character to the data register for transmission through UART
}

void transmit_tick() {
	transmit_char('T'); // Calling the transmit function to start transmission of characters.
	transmit_char('I');
	transmit_char('C');
	transmit_char('K');
	transmit_char('\r');
	transmit_char('\n');
}
void TIM1_UP_IRQHandler(void) {
	if (TIM1_SR & (1)) { // Checking if the timer 1 has overloaded by the timer 1 status register (bit 1) UIF (Update Interrupt Flag)
		if (i == 0) { // check if the led if off.
			GPIOA_BSRR = (1 << 5); // Write to Bit 5 of BSRR to turn LED ON
			i = 1; // Update LED state
		} else {
			GPIOA_BSRR = (1 << 21); // Write to the Bit 21 to turn led OFF
			i = 0; // Update LED state
		}
		transmit_tick();
		TIM1_SR &= ~(1); // The update interrupt flag set to 0 on completion of interrupt. (Clearing the pending flag)
	}
}

