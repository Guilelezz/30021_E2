#ifndef ADDAC_H_
#define ADDAC_H_

#include "stm32f30x.h"
#include <stdio.h>

void ADC_setup_PA();
uint16_t ADC_measure_PA(uint8_t ch);


#endif /* ADDAC_H_ */
