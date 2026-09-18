#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"


void exercise_2_3(void)
{
	//512 words
	//1024 halfwords
	//2048 bytes

	//writing words
	uint32_t tempval = 0xDEADBEEF;
	init_page_flash(PG31_BASE);
	FLASH_Unlock();
	for(uint8_t i = 0; i < 500; i++)
	{
		write_word_flash(PG31_BASE,i,tempval);
	}
	FLASH_Lock();


	//writing halfwords
	uint16_t tempval2 = 0xBEEF;
	init_page_flash(PG31_BASE); // flashing the page
	FLASH_Unlock();
	for(uint8_t i = 0; i< 10; i++)
	{
		write_hword_flash(PG31_BASE,i,tempval2);
	}
	FLASH_Lock();

    while (1)
    {



    }
}
