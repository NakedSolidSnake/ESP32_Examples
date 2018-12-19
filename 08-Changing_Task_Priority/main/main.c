#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

TaskHandle_t xTask2Handle = NULL;

void vTask1 (void *pvParameters)
{
	UBaseType_t uxPriority;

	uxPriority = uxTaskPriorityGet (NULL);

	for(;;)
	{
		printf("Task 1 is running\n");

		printf("About to raise the Task 2 priority\n");

		vTaskPrioritySet (xTask2Handle, (uxPriority + 1));
	}
}

void vTask2( void *pvParameters)
{
	UBaseType_t uxPriority;

	uxPriority = uxTaskPriorityGet (NULL);

	for(;;)
	{
		printf("Task 2 is running\n");

		printf("About to lower the Task 2 priority\n");

		vTaskPrioritySet(NULL, (uxPriority - 2));
	}
}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xTaskCreate (vTask1, "Task 1", 1000, NULL, 2, NULL);

	xTaskCreate (vTask2, "Task 2", 1000, NULL, 2, &xTask2Handle);
}