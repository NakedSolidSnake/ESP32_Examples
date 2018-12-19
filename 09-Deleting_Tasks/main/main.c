#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

TaskHandle_t xTask2Handle = NULL;

void vTask2 (void * pvParameters)
{
	printf("Task 2 is running and about ro delete itself\n");

	vTaskDelete ( xTask2Handle);
}

void vTask1 (void * pvParameters)
{
	const TickType_t xDelay100ms = pdMS_TO_TICKS(100);

	for(;;)
	{
		printf("Task 1 is running\n");

		xTaskCreate(vTask2, "Task 2", 1000, NULL , 2, &xTask2Handle);

		vTaskDelay( xDelay100ms );
	}
}




void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xTaskCreate (vTask1, "Task 1", 1000, NULL, 1, NULL)	;
}