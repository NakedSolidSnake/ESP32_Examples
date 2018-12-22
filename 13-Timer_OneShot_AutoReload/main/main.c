#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define mainONE_SHOT_TIMER_PERIOD 		pdMS_TO_TICKS(3333)
#define mainAUTO_RELOAD_TIMER_PERIOD	pdMS_TO_TICKS(500)

unsigned long ulCallCount = 0;

static void prvOneShotTimerCallback (TimerHandle_t xTimer)
{
	TickType_t xTimeNow;

	xTimeNow = xTaskGetTickCount();

	printf("One-shot timer callback executing %d\n", xTimeNow);
	ulCallCount++;
}


static void prvAutoReloadTimerCallback(TimerHandle_t xTimer)
{
	TickType_t xTimeNow;

	xTimeNow = xTaskGetTickCount();

	printf("Auto-Reload timer callback executing %d\n", xTimeNow);
	ulCallCount++;
}

void app_main()
{
	nvs_flash_init();
	/* Create Tasks here */	
	TimerHandle_t xAutoReloadTimer, xOneShotTimer;

	BaseType_t xTimer1Started, xTimer2Started;

	xOneShotTimer = xTimerCreate ("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvOneShotTimerCallback);

	xAutoReloadTimer = xTimerCreate ("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback);

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