#ifndef LCD_H
#define LCD_H

#include "stm32f30x_conf.h"

/* =========================
 * LCD Control Pins
 * ========================= */

// Chip Select
#define LCD_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define LCD_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_6)

// A0: Command/Data selection
#define LCD_A0_CMD()       GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LCD_A0_DATA()      GPIO_SetBits(GPIOA, GPIO_Pin_8)


/* =========================
 * LCD Initialization
 * ========================= */

void LCD_GPIO_Init(void);
void LCD_SPI_Init(void);
void LCD_Init(void);
void LCD_Reset(void);


/* =========================
 * LCD SPI
 * ========================= */

void LCD_SPI_Write(uint8_t data);


/* =========================
 * LCD Commands / Data
 * ========================= */

void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);


/* =========================
 * LCD Display Functions
 * ========================= */

// page: 0-3
// col: 0-127
void LCD_SetCursor(uint8_t page, uint8_t col);

// Write a single character
void LCD_WriteChar(char ch);

// Write a string starting at page/column
void LCD_WriteString(uint8_t page, uint8_t col, const char *str);

// Fill entire display
void LCD_Fill(void);

// Clear entire display
void LCD_Clear(void);


/* =========================
 * Font
 * ========================= */

// 5x7 font, ASCII 0x20 (' ') through 0x5F ('_')
extern const uint8_t Font5x7[][5];

#endif /* LCD_H */