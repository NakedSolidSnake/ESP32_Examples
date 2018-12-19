#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs_flash.h"

QueueHandle_t xQueue;

static void vSenderTask (void *pvParameters)
{
	int32_t lValueToSend;

	BaseType_t xStatus;

	lValueToSend = (int32_t) pvParameters;

	for(;;)
	{
		xStatus = xQueueSendToBack( xQueue , &lValueToSend, 0);

		if(xStatus != pdPASS)
		{
			printf("Couldn't send to the queue\n");
		}
	}
}

static void vReceiverTask (void *pvParameters)
{
	int32_t lReceivedValue;

	BaseType_t xStatus;

	const TickType_t xTicksToWait = pdMS_TO_TICKS (100);

	for(;;)
	{
		if(uxQueueMessagesWaiting(xQueue) != 0)
		{
			printf("Queue should have been empty!\n");
		}

		xStatus = xQueueReceive (xQueue, &lReceivedValue, xTicksToWait);

		if( xStatus == pdPASS)
		{
			printf("Received = %d\n", lReceivedValue);
		}
		else
		{
			printf("Couldn't receive from the queue\n");
		}
	}
}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	xQueue = xQueueCreate (5 , sizeof( int32_t));

	if(xQueue != NULL)
	{
		xTaskCreate(vSenderTask, "Sender1", 1000, (void *) 100, 1, NULL);
		xTaskCreate(vSenderTask, "Sender2", 1000, (void *) 200, 1, NULL);

		xTaskCreate(vReceiverTask, "Receiver", 2000, NULL, 2, NULL);		
	}

	else
	{

	}
}