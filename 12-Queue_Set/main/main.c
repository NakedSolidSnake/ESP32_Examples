#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs_flash.h"

static QueueHandle_t xQueue1 = NULL, xQueue2 = NULL;

static QueueSetHandle_t xQueueSet = NULL;

void vSenderTask1(void *pvParameters);
void vSenderTask2(void *pvParameters);
void vReceiverTask(void *pvParameters);

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	

	xQueue1 = xQueueCreate( 1 , sizeof(char *));

	xQueue2 = xQueueCreate( 1 , sizeof(char *));

	xQueueSet = xQueueCreateSet ( 1 * 2);

	xQueueAddToSet( xQueue1, xQueueSet );
	xQueueAddToSet( xQueue2, xQueueSet );

	xTaskCreate(vSenderTask1, "Sender1", 2048, NULL, 1, NULL);
	xTaskCreate(vSenderTask2, "Sender2", 2048, NULL, 1, NULL);

	xTaskCreate(vReceiverTask, "Receiver", 2048, NULL, 2, NULL);
	
}

void vSenderTask1(void *pvParameters)
{
	const TickType_t xBlockTime = pdMS_TO_TICKS(100);
	const char *const pcMessage = "Message from vSenderTask1\n";

	for(;;)
	{
		vTaskDelay (xBlockTime);
		xQueueSend( xQueue1, &pcMessage, 0);
	}
}

void vSenderTask2(void *pvParameters)
{

	const TickType_t xBlockTime = pdMS_TO_TICKS(200);
	const char *const pcMessage = "Message from vSenderTask2\n";

	for(;;)
	{
		vTaskDelay (xBlockTime);
		xQueueSend( xQueue2, &pcMessage, 0);
	}
}

void vReceiverTask(void *pvParameters)
{
	QueueHandle_t xQueueThatContainsData;

	char *pcReceivedString;

	for(;;)
	{
		xQueueThatContainsData = (QueueHandle_t) xQueueSelectFromSet (xQueueSet, portMAX_DELAY);

		xQueueReceive (xQueueThatContainsData, &pcReceivedString, 0);

		printf(pcReceivedString);
	}
}