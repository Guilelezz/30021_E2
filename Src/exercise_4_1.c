#include "lsm9ds1.c"

void exercise_4_1(void) {
	init_spi_lsm9ds1();

	uint8_t data_out;
	while (1) {
		data_out = ls9mds1_read(WHO_AM_I);
		printf("Received data = %X02\n", data_out);
	}
}
