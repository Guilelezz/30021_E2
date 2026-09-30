#ifndef GPIO_H_
#define GPIO_H_

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "stm32f30x.h"
#include "30010_io.h"

/* Global Flag Variables */
extern volatile uint8_t joystick_flag ;
extern volatile uint8_t current_joystick_state;

/* Joystick Direction Bitmasks */
#define JOYSTICK_UP  	 (1 << 0) // PA4
#define JOYSTICK_DOWN    (1 << 1) // PB0
#define JOYSTICK_LEFT    (1 << 2) // PC1
#define JOYSTICK_RIGHT   (1 << 3) // PC0
#define JOYSTICK_CENTER  (1 << 4) // PB5

/* Function Prototypes */
void initJoystick(void);
void initEXTI(void);
uint8_t readJoystick(void);

typedef enum
{
    LED_OFF,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_YELLOW,
    LED_CYAN,
    LED_MAGENTA,
    LED_WHITE
} LedColor;

void initLed(void);
void setLed(LedColor color);


#endif /* GPIO_H_ */
