// This is the library that controls the size of the datatypes for stm32
#include<stdint.h>

// defining the memory address as a pointed for the GPIOCRL,RCC, BSRR, ODR
#define RCC_BASE 0x40021000
#define GPIOA_BASE 0x40010800

// this is the respective RCC,BSRR,ODR and CRL with their respective offsets
#define RCC_APB2ENR *(unsigned int *) (RCC_BASE + 0x18)
#define GPIOA_CRL *(unsigned int *) (GPIOA_BASE )
#define GPIOA_BSRR *(unsigned int *) (GPIOA_BASE + 0x10)


int main(void){

	// the bit 2 of the RCC APB2 clock register is set to 1 so that the GPIO A port recieve clock signal
	RCC_APB2ENR |= (1<<2);
	// the configuration register is modified so that bit 20-23 is reset to 0
	GPIOA_CRL &= ~(0xf << 20);
	// the configuration register is modified so that the bit 20-23 is set to 0010 which is general purpose io and output for 2Mhz
	GPIOA_CRL |= (0x2 << 20);

	// The toggle one for pin 5 is made through setting the bit 5 of the Bit set reset register to 1
	GPIOA_BSRR = (1 << 5);


  // the pin5 is turned to logic low by setting the bit 5 of the output data register for GPIO A to 0
	GPIOA_BSRR = (1 << 21);


	
	return 0;
}
