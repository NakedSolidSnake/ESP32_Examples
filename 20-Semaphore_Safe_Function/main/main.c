#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "nvs_flash.h"

SemaphoreHandle_t xMutex;

static void prvNewPrintString( const char *pcString)
{
	xSemaphoreTake( xMutex, portMAX_DELAY);
	printf("%s\n", pcString );
	xSemaphoreGive(xMutex);
}

static void prvPrintTask(void *pvParameters)
{
	char *pcStringToPrint;

	const TickType_t xMaxBlockTimeTicks = 0x20;

	pcStringToPrint = (char *)pvParameters;

	for(;;)
	{
		prvNewPrintString(pcStringToPrint);
		vTaskDelay(rand() % xMaxBlockTimeTicks);
	}
}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xMutex = xSemaphoreCreateMutex();

	if( xMutex != NULL)
	{
		xTaskCreate (prvPrintTask, "Print1", 1000, "Task 1 ***********************************************\n", 1, NULL);
		xTaskCreate (prvPrintTask, "Print2", 1000, "Task 2 -----------------------------------------------\n", 2, NULL);
	}
}