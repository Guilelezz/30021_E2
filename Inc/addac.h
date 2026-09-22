#ifndef ADDAC_H_
#define ADDAC_H_

#include "stm32f30x.h"
#include <stdio.h>

#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30

void ADC_setup_PA();
uint16_t ADC_reference();
uint16_t ADC_measure_PA(uint8_t ch);


#endif /* ADDAC_H_ */
