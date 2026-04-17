#include <stdint.h>

#define RCC_BASE 0x40021000 // This is the base address of RCC Register from the RM0008 documentation
#define USART2_BASE 0x40004400 // This is the base address of UART2 Register from the RM0008 documentation
#define GPIOA_BASE 0x40010800 // This is the base address of GPIOA Register from the RM0008 documentation

#define RCC_APB1ENR *(unsigned int *) (RCC_BASE + 0x1C) // The APB1ENR register has an offset of 0x1C
#define RCC_APB2ENR *(unsigned int *) (RCC_BASE + 0x18) // The APB2ENR register has an offset of 0x18

#define USART2_BRR (*(unsigned int *) (USART2_BASE + 0x08)) // The UART2 Baud Rate Register(BRR) has an offset of 0x08
#define USART2_CR1 (*(unsigned int *) (USART2_BASE + 0x0C)) // The UART2 Control 1 (CR1) register has an offset of 0x0C
#define USART2_SR (*(unsigned int *) (USART2_BASE)) // The UART2 Status Register is at the base of UART2 Register
#define USART2_DR (*(unsigned int *) (USART2_BASE + 0x04)) // The UART2 Data register has an offset of 0x04
#define GPIOA_CRL (*(unsigned int *) (GPIOA_BASE)) // The GPIO Port A Configuration Register Low(CRL) is at the base of GPIOA

#define FCLK 8000000 // The default clock speed for STM32 at startup is 8Mhz
#define BAUDRATE 115200 // The desired baud rate for USART communication is 115200

#define USARTDIV  FCLK/(16*BAUDRATE) // USART DIV is the value that guides the controller how to split the clock to achieve the desired baud rate
// The USARTDIV comes out to be 4.34. in hex 0x4 is 4 and 0x5 which is 0.34 * 16 = 5.44 for scaling and rounded to 5. The final result to be added to BRR becomes 0x45.


// This function is used to burn the clock cycles to create delay.
void delay(volatile uint32_t count){
	while(count--);
}

void transmit_char(char chr){
	while(!(USART2_SR & (1<<7))); // Waiting for the Bit 7 of the Status register to be 1, to signal that new data is ready to be received.
	USART2_DR = chr; // Writing the character to the data register for transmission through UART
}
int main(void)
{

	RCC_APB1ENR |= (1<<17); // Enabling APB1 Bus in RCC for UART2
	RCC_APB2ENR |= (1<<2); // Enabling APB2 Bus in RCC for GPIO Port A

	GPIOA_CRL &= ~(0xf << 8); // Clearing the bits for PA2, Bit 8-12
	GPIOA_CRL |= (0xb << 8); // Configuring PA2 to be a Alternate Function Output at 50MHz. (1011).

	USART2_BRR = (0x45); // Configuring the baud rate register for 115200 using the UARTDIV value (0x45)
	USART2_CR1 |= (1 << 13); // Enabling the UART Enabling bit (Bit 13) in the UART Control Register
	USART2_CR1 |= (1 << 3); // Enabling the Transmission Enable (Bit 3) in the UART Control Register

	while(1){

		transmit_char('M'); // Calling the transmit function to start transmission of characters.
		transmit_char('I');
		transmit_char('L');
		transmit_char('O');
		transmit_char('N');
		transmit_char(',');
		transmit_char('N');
		transmit_char('U');
		transmit_char('C');
		transmit_char('L');
		transmit_char('O');
		transmit_char('-');
		transmit_char('O');
		transmit_char('N');
		transmit_char('L');
		transmit_char('I');
		transmit_char('N');
		transmit_char('E');
		transmit_char('\r');
		transmit_char('\n');

		delay(720000); // burning CPU cycles for delay.

		}
	return 0;
}
