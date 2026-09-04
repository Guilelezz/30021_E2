#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>

#include "lcd.h"

void exercise_lcd(void)
{
    LCD_GPIO_Init();
    LCD_SPI_Init();

    LCD_Init();
    LCD_Clear();

    LCD_WriteString(0, 0, "HOLGER");
    LCD_WriteString(1, 0, "STM32 + LCD");

    while (1)
    {
    }
}
