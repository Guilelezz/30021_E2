#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"
#include "addac.h"


void exercise_2_5(void)
{
	init_spi_lcd();
	ADC_setup_PA();

	uint16_t V_REF_CALC = ADC_reference();
	printf("V_ref: %u mV\n", V_REF_CALC);

	uint16_t adc1, adc2;
	float adc1vol, adc2vol;

	uint8_t fbuffer[512];
    char buffer[32];

    sprintf(buffer, "ADC1: 0",
			adc1);
	lcd_write_string(buffer, fbuffer, 0, 0);

    sprintf(buffer, "ADC2: 0",
			adc2);
	lcd_write_string(buffer, fbuffer, 0, 1);
	lcd_push_buffer(fbuffer);

    sprintf(buffer, " ");
	lcd_write_string(buffer, fbuffer, 0, 2);
    sprintf(buffer, " ");
	lcd_write_string(buffer, fbuffer, 0, 3);
	lcd_push_buffer(fbuffer);

    while (1)
    {
    	VREFINT_CAL;
    	adc1 = ADC_measure_PA(1);
    	adc1vol = 3.3/(4085)*adc1;
        sprintf(buffer, "ADC1: %4g V",
    			adc1vol);
    	lcd_write_string(buffer, fbuffer, 0, 0);

    	adc2 = ADC_measure_PA(2);
    	adc2vol = 3.3/(4085)*adc2;
        sprintf(buffer, "ADC2: %4g V",
    			adc2vol);
    	lcd_write_string(buffer, fbuffer, 0, 1);
    	lcd_push_buffer(fbuffer);

        sprintf(buffer, "V_REF_CALC: %4u mV", V_REF_CALC);
    	lcd_write_string(buffer, fbuffer, 0, 3);
    	lcd_push_buffer(fbuffer);
    }
}
