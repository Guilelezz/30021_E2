#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"

void exercise_2_1(void)
{
	init_spi_lcd();
	uint8_t fbuffer[512];
	lcd_write_string("line1", fbuffer, 0, 0);
	lcd_write_string("line2", fbuffer, 0, 1);
	lcd_write_string("line3", fbuffer, 0, 2);
	lcd_write_string("line4", fbuffer, 0, 3);
	lcd_push_buffer(fbuffer);

    while (1)
    {


    }
}
