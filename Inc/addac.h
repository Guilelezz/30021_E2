#ifndef ADDAC_H_
#define ADDAC_H_

#include "stm32f30x.h"
#include <stdio.h>

#define CALFACT_PAGE_ADDR   ((uint32_t)0x0800F800)
#define CALFACT0_OFFSET     0
#define CALFACT1_OFFSET     1

#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30


void ADC_setup_PA();
uint16_t ADC_reference();
uint16_t ADC_measure_PA(uint8_t ch);
float average_voltage(uint8_t channel);
float calibrated_voltage_PA(uint8_t channel);
void calibrate_adc();
void load_calibration(void);
void write_calfacts_flash(float cf0, float cf1);
float average_voltage(uint8_t channel);

#endif /* ADDAC_H_ */
