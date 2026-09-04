#include "led.h"
#include "stm32f30x.h"

void initLed(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable GPIO clocks
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    // Red - PB4
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Green - PC7
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Blue - PA9
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Start with LED off
    setLed(LED_OFF);
}


void setLed(LedColor color)
{
    // Turn all colours OFF
    GPIO_SetBits(GPIOB, GPIO_Pin_4);   // Red OFF
    GPIO_SetBits(GPIOC, GPIO_Pin_7);   // Green OFF
    GPIO_SetBits(GPIOA, GPIO_Pin_9);   // Blue OFF

    // Turn requested colour(s) ON
    switch (color)
    {
        case LED_RED:
            GPIO_ResetBits(GPIOB, GPIO_Pin_4);
            break;

        case LED_GREEN:
            GPIO_ResetBits(GPIOC, GPIO_Pin_7);
            break;

        case LED_BLUE:
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            break;

        case LED_YELLOW:
            GPIO_ResetBits(GPIOB, GPIO_Pin_4);
            GPIO_ResetBits(GPIOC, GPIO_Pin_7);
            break;

        case LED_CYAN:
            GPIO_ResetBits(GPIOC, GPIO_Pin_7);
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            break;

        case LED_MAGENTA:
            GPIO_ResetBits(GPIOB, GPIO_Pin_4);
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            break;

        case LED_WHITE:
            GPIO_ResetBits(GPIOB, GPIO_Pin_4);
            GPIO_ResetBits(GPIOC, GPIO_Pin_7);
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            break;

        case LED_OFF:
        default:
            // All colours are already OFF
            break;
    }
}
