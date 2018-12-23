#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define mainONE_SHOT_TIMER_PERIOD 		pdMS_TO_TICKS(3333)
#define mainAUTO_RELOAD_TIMER_PERIOD	pdMS_TO_TICKS(500)

TimerHandle_t xAutoReloadTimer, xOneShotTimer;

static void prvTimerCallback(TimerHandle_t xTimer )
{
	TickType_t xTimeNow;
	uint32_t ulExecutionCount;

	ulExecutionCount = (uint32_t) pvTimerGetTimerID(xTimer);

	ulExecutionCount++;

	vTimerSetTimerID(xTimer, (void *)ulExecutionCount);

	xTimeNow = xTaskGetTickCount();

	if(xTimer == xOneShotTimer)
	{
		printf("One-shot timer callback executing %d\n", xTimeNow);	
	}

	else
	{
		printf("Auto-Reload timer callback executing %d\n", xTimeNow);
		
		if(ulExecutionCount == 5)
		{
			xTimerStop(xTimer, 0);
		}	
	}

}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */		

	BaseType_t xTimer1Started, xTimer2Started;

	xOneShotTimer = xTimerCreate ("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvTimerCallback);

	xAutoReloadTimer = xTimerCreate ("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvTimerCallback);

	if( (xOneShotTimer != NULL) && (xAutoReloadTimer != NULL))
	{
		xTimer1Started = xTimerStart( xOneShotTimer, 0);
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0);

		if( (xTimer1Started == pdPASS) && (xTimer2Started == pdPASS) )
		{
			printf("Timers Started\n");
		}
	}
}