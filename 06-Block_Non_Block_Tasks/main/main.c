#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

void vContinuosProcessingTask( void * pvParameters)
{
	char *pcTaskName;

	pcTaskName = ( char *)pvParameters;

	for(;;)
	{
		printf("%s\n", pcTaskName);
	}
}

void vPeriodicTask( void *pvParameters)
{
	TickType_t xLastWakeTime;

	const TickType_t xDelay3ms = pdMS_TO_TICKS (3);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		printf("Periodic task is running\n");

		vTaskDelayUntil(&xLastWakeTime, xDelay3ms);
	}
}

const char *msg = "This is the Continuous task";

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xTaskCreate( vContinuosProcessingTask, "Continuous", 1000, (void *)msg, 1 , NULL);
	xTaskCreate( vPeriodicTask,            "Periodic"  , 1000, NULL, 2 , NULL);

}