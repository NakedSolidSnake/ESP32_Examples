/*
 * lock.c
 *
 *  Created on: 8 de out de 2018
 *      Author: solid
 */

#include "../include/locks.h"
#include <driver/gpio.h>

static gpio_config_t door[4];

int Door_Initialize(void)
{
	int i;
	gpio_config_t gpio_init = {
			.intr_type = GPIO_PIN_INTR_DISABLE,
			.mode = GPIO_MODE_OUTPUT,
			.pull_down_en = 0,
			.pull_up_en = 0
	};

	gpio_pad_select_gpio(DOOR_1);
  
    gpio_set_direction(DOOR_1, GPIO_MODE_OUTPUT);

	// for(i = 0; i < sizeof(door); i++)
	// {
	// 	door[i].intr_type = gpio_init.intr_type;
	// 	door[i].mode = gpio_init.mode;
	// 	door[i].pin_bit_mask = (1 << i);
	// 	door[i].pull_down_en = gpio_init.pull_down_en;
	// 	door[i].pull_up_en = gpio_init.pull_up_en;

	// 	gpio_config(&door[i]);

	// }
	return 0;
}

int Door_Open(DOORS door, DOOR_STATE state)
{
	if(state)
	{
		gpio_set_level(door, 0);
	}
	else
	{
		gpio_set_level(door, 1);
	}

	return 0;
}

