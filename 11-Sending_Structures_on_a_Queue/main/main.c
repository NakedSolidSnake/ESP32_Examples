#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs_flash.h"

typedef enum
{
	eSender1,
	eSender2
}DataSource_t;

typedef struct 
{
	uint8_t ucValue;
	DataSource_t eDataSource;
}Data_t;

static const Data_t xStructsToSend [2] = 
{
	{100, eSender1},
	{200, eSender2}
};


QueueHandle_t xQueue;

static void vSenderTask( void *pvParameters)
{
	BaseType_t xStatus;

	const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

	for(;;)
	{
		xStatus = xQueueSendToBack (xQueue, pvParameters, xTicksToWait);

		if(xStatus != pdPASS)
		{
			printf("Could not send to the queue.\n");
		}
	}
}

static void vReceiverTask(void * pvParameters)
{
	Data_t xReceivedStructure;

	BaseType_t xStatus;

	for(;;)
	{
		if (uxQueueMessagesWaiting (xQueue) != 3)
		{
			printf("Queue should have been full!\n");
		}

		xStatus = xQueueReceive (xQueue, &xReceivedStructure, 0);

		if (xStatus == pdPASS)
		{
			if(xReceivedStructure.eDataSource == eSender1)
			{
				printf("From Sender 1 = %d\n", xReceivedStructure.ucValue);
			}

			else{
				printf("From Sender 2 = %d\n", xReceivedStructure.ucValue);	
			}
		}

		else{
			printf("Couldn't receive from the queue.\n");
		}

	}
}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	

	xQueue = xQueueCreate (2 ,sizeof(Data_t));
	if(xQueue != NULL)
	{
		xTaskCreate(vSenderTask, "Sender1", 2000, &(xStructsToSend[0]), 2, NULL);
		xTaskCreate(vSenderTask, "Sender2", 2000, &(xStructsToSend[1]), 2, NULL);

		xTaskCreate(vReceiverTask, "Receiver", 2000, NULL, 1, NULL);
	}
	
}