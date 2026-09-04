#include "stm32f30x_conf.h"

void LCD_GPIO_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    // CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // RESET
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // A0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
}

void LCD_SPI_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

void LCD_SPI_Write(uint8_t data)
{
    while (!(SPI2->SR & SPI_SR_TXE));

    SPI_SendData8(SPI2, data);

    while (SPI2->SR & SPI_SR_BSY);
}

void LCD_Reset(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);

    for (volatile int i=0; i<100000; i++);

    GPIO_SetBits(GPIOB, GPIO_Pin_14);

    for (volatile int i=0; i<100000; i++);
}

#define LCD_CS_LOW()      GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define LCD_CS_HIGH()     GPIO_SetBits(GPIOB, GPIO_Pin_6)

#define LCD_A0_CMD()      GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LCD_A0_DATA()     GPIO_SetBits(GPIOA, GPIO_Pin_8)

void LCD_Command(uint8_t cmd)
{
    LCD_A0_CMD();

    LCD_CS_LOW();
    LCD_SPI_Write(cmd);
    LCD_CS_HIGH();
}

void LCD_Data(uint8_t data)
{
    LCD_A0_DATA();

    LCD_CS_LOW();
    LCD_SPI_Write(data);
}

void LCD_Init(void)
{
    LCD_Reset();

    LCD_Command(0xAE); // Display OFF

    LCD_Command(0xA2); // LCD bias 1/9
    LCD_Command(0xA0); // ADC normal
    LCD_Command(0xC8); // COM reverse
    LCD_Command(0x22); // Resistor ratio

    LCD_Command(0x2F); // Power control

    LCD_Command(0x40); // Display start line 0

    LCD_Command(0xAF); // Display ON
}

// 5x7 font table, covers ASCII 0x20 (space) through 0x5F (underscore)
// each glyph is 5 columns wide; bits 0-6 of each byte are the 7 pixel rows
const uint8_t Font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // (space)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x56, 0x20, 0x50}, // &
    {0x00, 0x08, 0x07, 0x03, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x2A, 0x1C, 0x7F, 0x1C, 0x2A}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x80, 0x70, 0x30, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x00, 0x60, 0x60, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x72, 0x49, 0x49, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x49, 0x4D, 0x33}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x31}, // 6
    {0x41, 0x21, 0x11, 0x09, 0x07}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x46, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x00, 0x14, 0x00, 0x00}, // :
    {0x00, 0x40, 0x34, 0x00, 0x00}, // ;
    {0x00, 0x08, 0x14, 0x22, 0x41}, //
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x59, 0x09, 0x06}, // ?
    {0x3E, 0x41, 0x5D, 0x59, 0x4E}, // @
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x41, 0x3E}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x1C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x03, 0x04, 0x78, 0x04, 0x03}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x00, 0x7F, 0x41, 0x41}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // backslash
    {0x41, 0x41, 0x7F, 0x00, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
};

// page: 0-3 (each page is 8px tall), col: 0-127
void LCD_SetCursor(uint8_t page, uint8_t col)
{
    LCD_Command(0xB0 + page);                 // page address
    LCD_Command(0x10 | ((col >> 4) & 0x0F));  // upper column nibble
    LCD_Command(0x00 | (col & 0x0F));         // lower column nibble
}

void LCD_WriteChar(char ch)
{
    if (ch >= 'a' && ch <= 'z') ch -= 0x20;   // no lowercase glyphs, use uppercase
    if (ch < 0x20 || ch > 0x5F) ch = 0x20;    // unsupported char -> blank

    const uint8_t *glyph = Font5x7[ch - 0x20];
    for (uint8_t i = 0; i < 5; i++)
    {
        LCD_Data(glyph[i]);
    }
    LCD_Data(0x00); // 1px gap between characters
}

// page: 0-3, col: 0-127 (start position)
void LCD_WriteString(uint8_t page, uint8_t col, const char *str)
{
    LCD_SetCursor(page, col);
    while (*str)
    {
        if (col > 128 - 6)              // no room left on this line
        {
            col = 0;
            page = (page + 1) % 4;
            LCD_SetCursor(page, col);
        }
        LCD_WriteChar(*str++);
        col += 6;
    }
}

void LCD_Fill(void)
{
	for(uint8_t page = 0; page < 4; page++)
	    {
	        LCD_SetCursor(page, 0);
	        for(uint8_t col = 0; col < 128; col++)
	        {
	            LCD_Data(0xFF);
	        }
	    }
}

void LCD_Clear(void)
{
    for(uint8_t page = 0; page < 4; page++)
    {
        LCD_SetCursor(page, 0);
        for(uint8_t col = 0; col < 128; col++)
        {
            LCD_Data(0x00);
        }
    }
}
