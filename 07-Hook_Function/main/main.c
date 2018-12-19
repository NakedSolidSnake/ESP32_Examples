#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

void vApplicationIdleHook(void);

volatile uint32_t ulIdleCycleCount = 0UL;

void vTaskFunction( void *pvParameters )
{
	// char *pcTaskName;

	const TickType_t xDelay250ms = pdMS_TO_TICKS(250);

	// pcTaskName = (char *)pvParameters;

	for(;;)
	{
		printf("ulIdleCycleCount = %d\n", ulIdleCycleCount);
		vTaskDelay( xDelay250ms);
	}


}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xTaskCreate(vTaskFunction, "vTaskFunction", 2048, NULL, 1, NULL);
	
}



void vApplicationIdleHook(void)
{	
	ulIdleCycleCount++;
}