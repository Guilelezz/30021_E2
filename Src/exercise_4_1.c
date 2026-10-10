#include "lsm9ds1.h"

void exercise_4_1(void) {
	init_spi_lsm9ds1();
	init_AG();
	int16_t temp;
	int16_t data_raw_xl[3];
	int16_t data_raw_gy[3];

	float xl_x, xl_y, xl_z;
	float gy_x, gy_y, gy_z;

	while (1) {
//		data_out16 = read_temp();

		// Read values from lsm9ds1
		read_xl(data_raw_xl);
		read_gy(data_raw_gy);

		// Convert values from int16 to float
		xl_x = fs4g_to_mg(data_raw_xl[0]);
		xl_y = fs4g_to_mg(data_raw_xl[1]);
		xl_z = fs4g_to_mg(data_raw_xl[2]);

		gy_x = fs2000dps_to_mdps(data_raw_gy[0]);
		gy_y = fs2000dps_to_mdps(data_raw_gy[1]);
		gy_z = fs2000dps_to_mdps(data_raw_gy[2]);

		printf("Accelerometer readings\tX = %4.2f [mg]\t Y = %4.2f [mg]\t Z = %4.2f [mg]\n", xl_x, xl_y, xl_z);
		printf("Gyro readings\tX = %4.2f [mdps]\t Y = %4.2f [mdps]\t Z = %4.2f [mdps]\n", gy_x, gy_y, gy_z);
//		printf("Received data = %X\n", data_out16);

//		data_out8 = AG_read8(WHO_AM_I);
//		printf("Received value = %X\n", data_out8);

//		data_out16 = lsm9ds1_read16(WHO_AM_I);
//		printf("Received data = %X\n", data_out16);

//		lsm9ds1_write(WHO_AM_I, 0xAA);
	}
}
