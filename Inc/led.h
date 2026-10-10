#ifndef LED_H
#define LED_H

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

#endif
