#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdlib.h>

static void vProductorTask( void *pvParameters );
static void vConsumidorTask( void *pvParameters );

traceString logger;
xQueueHandle xQueue;


int main( void )
{
	vTraceEnable(TRC_START);
	logger = xTraceRegisterString("Log");

    xQueue = xQueueCreate( 2, sizeof( long ) );
	if( xQueue != NULL )
	{
		xTaskCreate( vProductorTask, "Productor", 240, NULL, 1, NULL );
		xTaskCreate( vConsumidorTask, "Consumidor", 240, NULL, 2, NULL );

		vTaskStartScheduler();
	}
	else
	{
		vTracePrint(logger,"La cola no pudo ser creada.");
	}
	for( ;; );
	return 0;
}

static void vProductorTask( void *pvParameters )
{
	srand(xTaskGetTickCount());
	long lValueToSend;
	portBASE_TYPE xStatus;
	portTickType xLastWakeTime;
	for( ;; )
	{
		xLastWakeTime = xTaskGetTickCount();
		lValueToSend = rand() % 100;
		xStatus = xQueueSend( xQueue, &lValueToSend, 0 );
		if( xStatus != pdPASS )
		{
			vTracePrint(logger, "No se pudo enviar a la cola." );
		}
		vTracePrintF(logger,"envie %d",lValueToSend);
		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_RATE_MS ) );
	}
}

static void vConsumidorTask( void *pvParameters )
{
long lReceivedValue;
portBASE_TYPE xStatus;
const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	for( ;; )
	{
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
		if( xStatus == pdPASS )
		{
			vTracePrintF(logger,"recibi  %d", lReceivedValue );
		}
		else
		{
			vTracePrint(logger, "No se pudo recibir de la cola");
		}
		vTaskDelay( 250 / portTICK_RATE_MS );
	}
}

void vApplicationMallocFailedHook( void )
{
	for( ;; );
}

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	for( ;; );
}

void vApplicationIdleHook( void )
{

}

void vApplicationTickHook( void )
{

}

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* Power up and feed the timer. */
	LPC_SC->PCONP |= 0x02UL;
	LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

	/* Reset Timer 0 */
	LPC_TIM0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIM0->CTCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
	but not too fast so as to overflow all the time. */
	LPC_TIM0->PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIM0->TCR = TCR_COUNT_ENABLE;
}



