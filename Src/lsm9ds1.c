#include "lsm9ds1.h"
#include <stdbool.h>
#include "stdint.h"
#include "lsm9ds1_reg.h"

void init_spi_lsm9ds1(void) {
	// Enable Clocks
	RCC->AHBENR |= 0x00020000 | 0x00040000; // Enable Clock for GPIO Banks A and B
	RCC->APB1ENR |= 0x00004000;                 // Enable Clock for SPI2

	// Connect pins to SPI2
	GPIOB->AFR[13 >> 0x03] &= ~(0x0000000F << ((13 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[13 >> 0x03] |= (0x00000005 << ((13 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - SCLK
	GPIOB->AFR[15 >> 0x03] &= ~(0x0000000F << ((15 & 0x00000007) * 4)); // Clear alternate function for PB15
	GPIOB->AFR[15 >> 0x03] |= (0x00000005 << ((15 & 0x00000007) * 4)); // Set alternate 5 function for PB15 - MOSI

	// Configure pins PB13 and PB15 for 10 MHz alternate function
	GPIOB->OSPEEDR &= ~(0x00000003 << (13 * 2) | 0x00000003 << (15 * 2)); // Clear speed register
	GPIOB->OSPEEDR |= (0x00000001 << (13 * 2) | 0x00000001 << (15 * 2)); // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER &= ~(0x0001 << (13) | 0x0001 << (15)); // Clear output type register
	GPIOB->OTYPER |= (0x0000 << (13) | 0x0000 << (15)); // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER &= ~(0x00000003 << (13 * 2) | 0x00000003 << (15 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000002 << (13 * 2) | 0x00000002 << (15 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR &= ~(0x00000003 << (13 * 2) | 0x00000003 << (15 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000000 << (13 * 2) | 0x00000000 << (15 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	// Initialize REEST, nCS, and A0
	// PB14 = SPI2 MISO
	GPIOB->AFR[14 >> 3] &= ~(0xF << ((14 & 7) * 4));
	GPIOB->AFR[14 >> 3] |= (0x5 << ((14 & 7) * 4));

	GPIOB->MODER &= ~(0x3 << (14 * 2));
	GPIOB->MODER |= (0x2 << (14 * 2));       // Alternate function

	GPIOB->PUPDR &= ~(0x3 << (14 * 2));
	// optionally:
	// GPIOB->PUPDR |=  (0x1 << (14 * 2));    // pull-up, if appropriate

	// Configure pins PB6 and PB10 for 10 MHz output
	GPIOB->OSPEEDR &= ~(0x00000003 << (6 * 2) | 0x00000003 << (10 * 2)); // Clear speed register
	GPIOB->OSPEEDR |= (0x00000001 << (6 * 2) | 0x00000001 << (10 * 2)); // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER &= ~(0x0001 << (6) | 0x0001 << (10)); // Clear output type register
	GPIOB->OTYPER |= (0x0000 << (6) | 0x0000 << (10)); // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER &= ~(0x00000003 << (6 * 2) | 0x00000003 << (10 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000001 << (6 * 2) | 0x00000001 << (10 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR &= ~(0x00000003 << (6 * 2) | 0x00000003 << (10 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000000 << (6 * 2) | 0x00000000 << (10 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	GPIOB->ODR |= (0x0001 << 6); // CS = 1
	GPIOB->ODR |= (0x0001 << 10); // D10 = 1

	// Configure SPI2
	SPI2->CR1 &= 0x3040; // Clear CR1 Register
	SPI2->CR1 |= 0x0000; // Configure direction (0x0000 - 2 Lines Full Duplex, 0x0400 - 2 Lines RX Only, 0x8000 - 1 Line RX, 0xC000 - 1 Line TX)
	SPI2->CR1 |= 0x0104; // Configure mode (0x0000 - Slave, 0x0104 - Master)
	SPI2->CR1 |= 0x0002; // Configure clock polarity (0x0000 - Low, 0x0002 - High)
	SPI2->CR1 |= 0x0001; // Configure clock phase (0x0000 - 1 Edge, 0x0001 - 2 Edge)
	SPI2->CR1 |= 0x0200; // Configure chip select (0x0000 - Hardware based, 0x0200 - Software based)
	SPI2->CR1 |= 0x0008; // Set Baud Rate Prescaler (0x0000 - 2, 0x0008 - 4, 0x0018 - 8, 0x0020 - 16, 0x0028 - 32, 0x0028 - 64, 0x0030 - 128, 0x0038 - 128)
	SPI2->CR1 |= 0x0000; // Set Bit Order (0x0000 - MSB First, 0x0080 - LSB First)
	SPI2->CR2 &= ~0x0F00; // Clear CR2 Register
	SPI2->CR2 |= 0x0700; // Set Number of Bits (0x0300 - 4, 0x0400 - 5, 0x0500 - 6, ...);
	SPI2->I2SCFGR &= ~0x0800; // Disable I2S
	SPI2->CRCPR = 7; // Set CRC polynomial order
	SPI2->CR2 &= ~0x1000;
	SPI2->CR2 |= 0x1000; // Configure RXFIFO return at (0x0000 - Half-full (16 bits), 0x1000 - Quarter-full (8 bits))
	SPI2->CR1 |= 0x0040; // Enable SPI2
}

uint8_t M_read8(uint8_t addr) {
	uint8_t data_out8;

	GPIOB->ODR &= ~(1 << 10);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {}
	SPI_SendData8(SPI2, addr | 0x80);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {}
	SPI_ReceiveData8(SPI2);

	SPI_SendData8(SPI2, 0x00);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET) {}
	data_out8 = SPI_ReceiveData8(SPI2);

	GPIOB->ODR |= (1 << 10);

	return data_out8;
}

uint8_t AG_read8(uint8_t addr) {
	uint8_t data_out8;

    GPIOB->ODR &= ~(1 << 6);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {}
	SPI_SendData8(SPI2, addr | 0x80);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {}
	SPI_ReceiveData8(SPI2);

	SPI_SendData8(SPI2, 0x00);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET) {}
	data_out8 = SPI_ReceiveData8(SPI2);

	GPIOB->ODR |= (1 << 6);

	return data_out8;
}

void M_write(uint8_t addr, uint8_t data_in) {
	// Transmit
	GPIOB->ODR &= ~(1 << 10);
	// Send address byte
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {
	}
	SPI_SendData8(SPI2, addr);
	// Send data_in to lsm9ds1
	SPI_SendData8(SPI2, data_in);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {
	}

	GPIOB->ODR |= (1 << 10);        // CS high
}

void AG_write(uint8_t addr, uint8_t data_in) {
	GPIOB->ODR &= ~(1 << 6);

	SPI_SendData8(SPI2, addr);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {}
	SPI_ReceiveData8(SPI2);

	SPI_SendData8(SPI2, data_in);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) {}
	SPI_ReceiveData8(SPI2);

	GPIOB->ODR |= (1 << 6);        // CS high
}


uint16_t read_temp(void) {
	uint8_t high_temp = AG_read8(0x16);
	uint8_t low_temp = AG_read8(0x15);
	uint8_t stat = AG_read8(0x17);

	int16_t temp_raw = ((int16_t)high_temp << 8) | low_temp;
	float temp = 25.0f + (temp_raw / 16.0f);

	printf("hi: %X  ",high_temp);
	printf("low: %X  ",low_temp);
//	printf("status: %X\n", stat);
	printf("total_temp = %.2f\n",temp);

	return temp_raw;
}

