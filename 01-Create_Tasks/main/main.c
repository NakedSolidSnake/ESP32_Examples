#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define mainDELAY_LOOP_COUNT 500000


void vTask1(void *pvParameters)
{

	const char *pcTaskName = "Task1 is running";

	volatile uint32_t ul;

	for(;;)
	{
		printf("%s\n", pcTaskName);

		for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++)
		{

		}
	}
}


void vTask2(void *pvParameters)
{

	const char *pcTaskName = "Task2 is running";

	volatile uint32_t ul;

	for(;;)
	{
		printf("%s\n", pcTaskName);

		for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++)
		{
			
		}
	}
}


void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xTaskCreate(vTask1, "Task 1", 1000, NULL, 1, NULL);
	xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);
	
}