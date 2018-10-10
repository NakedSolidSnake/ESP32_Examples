#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "include/locks.h"

#define BLINK_GPIO 13

void hello_task(void *pvParameters)
{
  while(1)
  { 
    printf("Hello world!\n");
    vTaskDelay(100/portTICK_RATE_MS);
  }
} 

void blink(void *pvParameters)
{
  DOOR_STATE door = DOOR_OPEN;
  // gpio_pad_select_gpio(BLINK_GPIO);
  
  // gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  // while(1)
  // {
  //   gpio_set_level(BLINK_GPIO, 0);
  //   vTaskDelay(1000 / portTICK_RATE_MS);
    
  //   gpio_set_level(BLINK_GPIO, 1);
  //   vTaskDelay(1000 / portTICK_RATE_MS);

  // }
  Door_Initialize();
   while(1)
   {
     Door_Open(DOOR_1, DOOR_OPEN);     
     vTaskDelay(1000 / portTICK_RATE_MS);
     Door_Open(DOOR_1, DOOR_CLOSE);     
     vTaskDelay(10000 / portTICK_RATE_MS);
   }
}

void app_main(void)
{
  nvs_flash_init();
  xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
  xTaskCreate(&blink, "blink", 512, NULL, 5 , NULL);
}
