#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"
#include "addac.h"


void exercise_2_4(void)
{
	init_spi_lcd();
	ADC_setup_PA();

	uint16_t adc1, adc2;

	uint8_t fbuffer[512];
    char buffer[32];

    sprintf(buffer, "ADC1: 0000",
			adc1);
	lcd_write_string(buffer, fbuffer, 0, 0);

    sprintf(buffer, "ADC2: 0000",
			adc2);
	lcd_write_string(buffer, fbuffer, 0, 1);
	lcd_push_buffer(fbuffer);


    while (1)
    {
    	adc1 = ADC_measure_PA(1);
        sprintf(buffer, "ADC1: %u",
    			adc1);
    	lcd_write_string(buffer, fbuffer, 0, 0);


    	adc2 = ADC_measure_PA(2);
        sprintf(buffer, "ADC2: %u",
    			adc2);
    	lcd_write_string(buffer, fbuffer, 0, 1);
    	lcd_push_buffer(fbuffer);


    }
}
