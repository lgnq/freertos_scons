/*
 * FreeRTOS V202011.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks
 * (which just exist to test the kernel port and provide an example of how to use
 * each FreeRTOS API function).
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" hook -  This only executes fully every five seconds from the tick
 * hook.  Its main function is to check that all the standard demo tasks are
 * still operational.  The status can be viewed using on the Task Stats page
 * served by the WEB server.
 *
 * "uIP" task -  This is the task that handles the uIP stack.  All TCP/IP
 * processing is performed in this task.
 *
 * "USB" task - Enumerates the USB device as a CDC class, then echoes back all
 * received characters with a configurable offset (for example, if the offset
 * is 1 and 'A' is received then 'B' will be sent back).  A dumb terminal such
 * as Hyperterminal can be used to talk to the USB task.
 */

/* Standard includes. */
#include "stdio.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "drv_led.h"


/* The time between cycles of the 'check' functionality (defined within the
tick hook. */
#define mainCHECK_DELAY ((TickType_t)5000 / portTICK_PERIOD_MS)

/* The toggle rate for the LED. */
#define LED0_TOGGLE_RATE					( ( TickType_t ) 1000 / portTICK_PERIOD_MS )
#define LED1_TOGGLE_RATE					( ( TickType_t ) 100 / portTICK_PERIOD_MS )

/* Task priorities. */
#define mainFLASH_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

/* The WEB server has a larger stack as it utilises stack hungry string
handling library calls. */
#define mainBASIC_WEB_STACK_SIZE (configMINIMAL_STACK_SIZE * 4)

/* The message displayed by the WEB server when all tasks are executing
without an error being reported. */
#define mainPASS_STATUS_MESSAGE "All tasks are executing without error."

/* Bit definitions. */
#define PCONP_PCGPIO 0x00008000
#define PLLFEED_FEED1 0x000000AA
#define PLLFEED_FEED2 0x00000055

/*
 * Configure the hardware for the demo.
 */
static void prvSetupHardware(void);

/*
 * Simply returns the current status message for display on served WEB pages.
 */
char *pcGetTaskStatusMessage(void);


/* Holds the status message displayed by the WEB server. */
static char *pcStatusMessage = mainPASS_STATUS_MESSAGE;

static void led0_task(void *pvParameters)
{
	TickType_t xLastFlashTime;

	/* We need to initialise xLastFlashTime prior to the first call to
	vTaskDelayUntil(). */
	xLastFlashTime = xTaskGetTickCount();

	for (;;)
	{
		led_on(0);
		/* Simply toggle the LED between delays. */
		vTaskDelayUntil(&xLastFlashTime, LED0_TOGGLE_RATE);
		
		led_off(0);
		/* Simply toggle the LED between delays. */
		vTaskDelayUntil(&xLastFlashTime, LED0_TOGGLE_RATE);
	}
}

static void led1_task(void *pvParameters)
{
	TickType_t xLastFlashTime;

	/* We need to initialise xLastFlashTime prior to the first call to
	vTaskDelayUntil(). */
	xLastFlashTime = xTaskGetTickCount();

	for (;;)
	{
		led_on(1);
		/* Simply toggle the LED between delays. */
		vTaskDelayUntil(&xLastFlashTime, LED1_TOGGLE_RATE);
		
		led_off(1);
		/* Simply toggle the LED between delays. */
		vTaskDelayUntil(&xLastFlashTime, LED1_TOGGLE_RATE);
	}
}

int main(void)
{
	/* Configure the hardware for use by this demo. */
	prvSetupHardware();

	/* Create the simple LED flash task. */
	xTaskCreate(led0_task, "LED0", configMINIMAL_STACK_SIZE, (void *)NULL, mainFLASH_TASK_PRIORITY, NULL);
	xTaskCreate(led1_task, "LED1", configMINIMAL_STACK_SIZE, (void *)NULL, mainFLASH_TASK_PRIORITY, NULL);

	/* Display the IP address, then create the uIP task.  The WEB server runs
	in this task.  --- Due to tool changes since this demo was created the LCD
	is no longer used.
	LCDdriver_initialisation();
	LCD_PrintString( 5, 10, "FreeRTOS.org", 14, COLOR_GREEN);
	sprintf( cIPAddress, "%d.%d.%d.%d", configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3 );
	LCD_PrintString( 5, 30, cIPAddress, 14, COLOR_RED);
    xTaskCreate( vuIP_Task, "uIP", mainBASIC_WEB_STACK_SIZE, ( void * ) NULL, mainUIP_TASK_PRIORITY, NULL ); */

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
    task.  The idle task is created within vTaskStartScheduler(). */
	for (;;)
		;
}

void vApplicationTickHook(void)
{
	static unsigned long ulTicksSinceLastDisplay = 0;

	/* Called from every tick interrupt as described in the comments at the top
	of this file.

	Have enough ticks passed to make it	time to perform our health status
	check again? */
	ulTicksSinceLastDisplay++;
	if (ulTicksSinceLastDisplay >= mainCHECK_DELAY)
	{
		/* Reset the counter so these checks run again in mainCHECK_DELAY
		ticks time. */
		ulTicksSinceLastDisplay = 0;

		// /* Has an error been found in any task? */
		// if (xAreGenericQueueTasksStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Generic Queue test/demo.";
		// }
		// else if (xAreQueuePeekTasksStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Peek Queue test/demo.";
		// }
		// else if (xAreBlockingQueuesStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Block Queue test/demo.";
		// }
		// else if (xAreBlockTimeTestTasksStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Block Time test/demo.";
		// }
		// else if (xAreSemaphoreTasksStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Semaphore test/demo.";
		// }
		// else if (xArePollingQueuesStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Poll Queue test/demo.";
		// }
		// else if (xAreIntegerMathsTaskStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Int Math test/demo.";
		// }
		// else if (xAreRecursiveMutexTasksStillRunning() != pdTRUE)
		// {
		// 	pcStatusMessage = "An error has been detected in the Mutex test/demo.";
		// }
	}
}

char *pcGetTaskStatusMessage(void)
{
	/* Not bothered about a critical section here. */
	return pcStatusMessage;
}

void prvSetupHardware(void)
{
	/* Configure the LEDs. */
	led_init();
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	/* This function will get called if a task overflows its stack. */

	(void)pxTask;
	(void)pcTaskName;

	for (;;)
		;
}

void vConfigureTimerForRunTimeStats(void)
{
	const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* This function configures a timer that is used as the time base when
	collecting run time statistical information - basically the percentage
	of CPU time that each task is utilising.  It is called automatically when
	the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
	to 1). */

	/* Power up and feed the timer. */
	LPC_SC->PCONP |= 0x02UL;
	LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3 << 2))) | (0x01 << 2);

	/* Reset Timer 0 */
	LPC_TIM0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIM0->CTCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
	but not too fast so as to overflow all the time. */
	LPC_TIM0->PR = (configCPU_CLOCK_HZ / 10000UL) - 1UL;

	/* Start the counter. */
	LPC_TIM0->TCR = TCR_COUNT_ENABLE;
}
