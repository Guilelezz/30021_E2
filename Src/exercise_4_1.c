#include "lsm9ds1.h"

void exercise_4_1(void) {
	init_spi_lsm9ds1();

	uint8_t data_out;
	while (1) {
		data_out = lsm9ds1_read8(WHO_AM_I);
		printf("Received data = %X02\n", data_out);
	}
}
