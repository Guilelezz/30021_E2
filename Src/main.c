#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>

#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "exercises.h"

int main(void) {
	uart_init(9600);

	exercise_1_4();

	while(1)
	{
	}
}
