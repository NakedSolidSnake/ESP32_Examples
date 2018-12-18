#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define mainDELAY_LOOP_COUNT 100000

void vTaskFunction(void *pvParameters)
{
	char *pcTaskName;

	const TickType_t xDelay250ms = pdMS_TO_TICKS(250);

	volatile uint32_t ul;

	pcTaskName = (char *)pvParameters;

	for(;;)
	{
		printf("%s\n", pcTaskName);
		vTaskDelay(xDelay250ms);		
	}
}

static const char *pcTextForTask1  = "Task 1 is running";
static const char *pcTextForTask2  = "Task 2 is running";

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xTaskCreate(&vTaskFunction, "Task 1", 1000, (void *)pcTextForTask1, 1, NULL);
	xTaskCreate(&vTaskFunction, "Task 2", 1000, (void *)pcTextForTask2, 2, NULL);
	
}