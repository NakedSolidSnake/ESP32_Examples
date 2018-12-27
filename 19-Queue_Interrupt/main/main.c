#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */

#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_IO_1    19
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

void Gpio_Init(void);
void Gpio_Intr_Config(void);

static void vStringPrinter (void *pvParameters);
static void vIntegerGenerator (void *pvParameters);

QueueHandle_t xIntegerQueue, xStringQueue;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    BaseType_t xHigherPriorityTaskWoken;

    static uint32_t ulReceivedNumber;

    xHigherPriorityTaskWoken = pdFALSE;

    static const char *pcString[] = {
    	"String 0\r\n",
    	"String 1\r\n",
    	"String 2\r\n",
    	"String 3\r\n",
    };    
   
   	while( xQueueReceiveFromISR( xIntegerQueue, &ulReceivedNumber, &xHigherPriorityTaskWoken) != errQUEUE_EMPTY)
   	{
   		ulReceivedNumber &= 0x03;

   		xQueueSendToBackFromISR( xStringQueue, &pcString[ulReceivedNumber], &xHigherPriorityTaskWoken);
   	}
    
    // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR();
}


void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	

	Gpio_Init();       
    Gpio_Intr_Config();

    xIntegerQueue = xQueueCreate( 10, sizeof(uint32_t));

    xStringQueue = xQueueCreate(10, sizeof(char *));

    xTaskCreate (vIntegerGenerator, "IntGen", 1024, NULL, 1, NULL);
    xTaskCreate (vStringPrinter, "String", 1024, NULL, 2, NULL);
	
}

static gpio_config_t io_conf;

void Gpio_Init(void)
{	
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);
}

void Gpio_Intr_Config(void)
{
	//install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
}

static void vStringPrinter (void *pvParameters)
{
	char *pcString;

	for(;;)
	{
		xQueueReceive( xStringQueue, &pcString, portMAX_DELAY);
		printf("%s\n", pcString);
	}
}

static void vIntegerGenerator (void *pvParameters)
{
	TickType_t xLastExecutionTime;

	uint32_t ulValueToSend = 0;

	int i;

	xLastExecutionTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil(&xLastExecutionTime, pdMS_TO_TICKS (200));

		for(i = 0; i < 5; i++)
		{
			xQueueSendToBack(xIntegerQueue, &ulValueToSend, 0);
			ulValueToSend++;
			if(ulValueToSend == 5)
				ulValueToSend = 0;
		}

		printf("Generator task - About to generate an interrupt\n");
	}
}