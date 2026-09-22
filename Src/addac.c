#include "stm32f30x.h"
#include <stdio.h>
#include "addac.h"

void ADC_setup_PA(){
	//PA0 & PA1 as input pins for potentiometers
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div8);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	ADC_InitTypeDef ADC_InitStruct;

	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ExternalTrigConvEvent = 0;
	ADC_InitStruct.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_OverrunMode = DISABLE;
	ADC_InitStruct.ADC_AutoInjMode = DISABLE;
	ADC_InitStruct.ADC_NbrOfRegChannel = 1;

	//sequencer length

	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_Cmd(ADC1,ENABLE);



	//calibration
	ADC_VoltageRegulatorCmd(ADC1,ENABLE);
	for(uint32_t i = 0; i<100;i++);

	ADC_Cmd(ADC1,DISABLE);
	while(ADC_GetDisableCmdStatus(ADC1)){} // wait for disable of ADC
	ADC_SelectCalibrationMode(ADC1,ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)){}
	for(uint32_t i = 0; i<100;i++);

	//enable ADC
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_RDY)){}


}

uint16_t ADC_reference(){
	ADC_Cmd(ADC1,DISABLE);
	ADC_VrefintCmd(ADC1,ENABLE); // setup ref voltage to channel 18
	for(uint32_t i = 0; i<10000;i++); // I think this is needed...
	ADC_Cmd(ADC1,ENABLE);
	// turn on ADC

	ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_601Cycles5);
	for(uint32_t i = 0; i<10000;i++); // I think this is needed...
	ADC_StartConversion(ADC1); // Start ADC read

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); // Wait for ADC read
	uint16_t sampled_value = ADC_GetConversionValue(ADC1);
	uint16_t vref_calc = 0;
	vref_calc = (uint16_t) (3300 * ((float)VREFINT_CAL/(float)sampled_value));
	ADC_VrefintCmd(ADC1,DISABLE); // setup ref voltage to channel 18
	return vref_calc;
}

uint16_t ADC_measure_PA(uint8_t ch){
	if(ch == 1){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
	}

	if(ch == 2){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
	}

	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); // Wait for ADC read

	return ADC_GetConversionValue(ADC1);
}




