/*CMSIS includes*/
#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

/* FreeRTOS.org includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
 /*	Stdlib includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Parametros */
#define MAX_INPUT_SIZE 30
#define USR_PRIO 4
#define TEMP_PRIO 2
#define TX_PRIO 3
#define MAX_QUEUE_SIZE 5
#define TX_BAUD_RATE 9600



void vUart_config();
void vPinsel_config();
/*	Tasks	*/
static void vTaskSensor(void * pvParameters);
static void vTaskUsuario(void * pvParameters);
static void vTaskTx(void * pvParameters);

typedef struct{
	char * msg;
	uint8_t temp;
	int msg_len;
}xInput;

xQueueHandle xQueue;
traceString logger;

int main( void )
{
 	/* Initialize hardware */
	vUart_config();
	vPinsel_config();

	/* Initialize tracealyzer*/
	vTraceEnable(TRC_START);
	logger = xTraceRegisterString("Log");
    /* The queue is created to hold a maximum of 5 long values. */

	/* Crear la cola */
	xQueue = xQueueCreate( MAX_QUEUE_SIZE, sizeof( xInput *) );

	if( xQueue != NULL )
	{
		/*Crear las 3 tareas*/
		xTaskCreate( vTaskTx, ( char * ) "Tx", 240, NULL, TX_PRIO, NULL );
		xTaskCreate( vTaskSensor, ( char * ) "Sensor", 240, NULL, TEMP_PRIO, NULL );
		xTaskCreate( vTaskUsuario, ( char * ) "User", 240, NULL, USR_PRIO, NULL );
	}
	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();
    /* If all is well we will never reach here as the scheduler will now be
    running the tasks.  If we do reach here then it is likely that there was
    insufficient heap memory available for a resource to be created. */
	for( ;; );
	return 0;
}

static void vTaskSensor(void * pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();

	/* Define a task that performs an action every random milisecons starting*/
	const TickType_t xPeriod = pdMS_TO_TICKS(1000); //100-200 ms

	portBASE_TYPE xStatus;
	for (;;)
	{
		xInput *in = pvPortMalloc(sizeof(xInput *));

		in->temp = rand() % 256;
		in->msg = NULL;

		xStatus = xQueueSendToBack( xQueue, &in, pdMS_TO_TICKS(1)); //100
		if( xStatus != pdPASS )
		{
			vTracePrint(logger,"No se pudo enviar el dato a la cola." );
		}
		vPortFree(in);
		vTracePrint(logger, "temperatura");

		vTaskDelayUntil( &xLastWakeTime, xPeriod ); //Periodico cada 1000ms			//usar vTaskDelayUntil
	}
}

static void vTaskUsuario(void * pvParameters)
{
	srand(xTaskGetTickCount());
	xInput * in = pvPortMalloc(sizeof(xInput *));

	for (;;)
	{
		int i;
		int cantidad = (rand() % MAX_INPUT_SIZE) + 1 ;
		in -> msg = pvPortMalloc(sizeof(char *) * cantidad);
		for(i=0; i < cantidad; i++)
		{
			char  aux = (rand() % 25) + 97; //25 letras, 97 base de ascii para minusculas
			in -> msg[i] = aux;
		}
		in -> msg_len = cantidad;
		vTracePrint(logger,"usuario");
		xQueueSend(xQueue, &in, 100);
		vPortFree(in->msg);
		vTaskDelay(( (rand() % 2000) + 2000) / portTICK_RATE_MS );
	}
}


static void vTaskTx(void * pvParameters)
{
	portBASE_TYPE xStatus;
	xInput *xIn = pvPortMalloc(sizeof(xInput *));
	char xStr[4];
	for (;;)
	{
		xStatus = xQueueReceive( xQueue, &xIn, portMAX_DELAY );
		if( xStatus == pdPASS )
		{
			if(xIn->msg == NULL)
			{
				sprintf(xStr,"%3d\n",(uint8_t)xIn->temp);
				UART_Send(LPC_UART0,(unsigned char *)xStr,sizeof(xStr),NONE_BLOCKING);
				vTracePrintF(logger,"Uart: %d",(int) xIn->temp);
			}
			else
			{
				char cMsg [(xIn->msg_len)+1];
				sprintf(cMsg,"%s\n",xIn->msg);
				UART_Send(LPC_UART0,(unsigned char *) cMsg,xIn->msg_len+1, NONE_BLOCKING);
				vTracePrintF(logger,"Uart: %s",xIn->msg);
			}
		}else
		{
			vTracePrint(logger,"No se pudo leer de la cola");
		}
	}
}

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	vTracePrint(logger,"ApplicationMallocFailed");
}

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	vTracePrint(logger,"ApplicationStackOverflow");
}

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;
	/* This function configures a timer that is used as the time base when
	collecting run time statistical information - basically the percentage
	of CPU time that each task is utilising.  It is called automatically when
	the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
	to 1). */

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

void vPinsel_config() {
	PINSEL_CFG_Type pin_config;
	// pin 0.11 RXD2
	pin_config.Portnum = PINSEL_PORT_0;
	pin_config.Pinnum = PINSEL_PIN_3;
	pin_config.Pinmode = PINSEL_PINMODE_PULLUP;
	pin_config.Funcnum = PINSEL_FUNC_1;
	pin_config.OpenDrain = PINSEL_PINMODE_TRISTATE;
	PINSEL_ConfigPin(&pin_config);
	GPIO_SetDir(0, (1<<11), 0);
	// pin 0.10 TXD2
	pin_config.Portnum = PINSEL_PORT_0;
	pin_config.Pinnum = PINSEL_PIN_2;
	pin_config.Pinmode = PINSEL_PINMODE_PULLUP;
	pin_config.Funcnum = PINSEL_FUNC_1;
	pin_config.OpenDrain = PINSEL_PINMODE_TRISTATE;
	PINSEL_ConfigPin(&pin_config);
	GPIO_SetDir(0, (1<<10), 1);
}

void vUart_config() {
	UART_CFG_Type struct_uart;
	UART_FIFO_CFG_Type struct_fifo;
	UART_ConfigStructInit(&struct_uart);
	struct_uart.Baud_rate = TX_BAUD_RATE;
	struct_uart.Databits = UART_DATABIT_8;
	UART_Init(LPC_UART0, &struct_uart);
	UART_FIFOConfigStructInit(&struct_fifo);
	UART_FIFOConfig(LPC_UART0, &struct_fifo);
	UART_IntConfig(LPC_UART0,UART_INTCFG_RBR,ENABLE);
	UART_TxCmd(LPC_UART0, ENABLE);
}


