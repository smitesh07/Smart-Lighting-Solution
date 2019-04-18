/*
 * led_task.c
 *
 *  Created on: Apr 5, 2019
 *      Author: modak
 */

#include "led_task.h"
#include "log_task.h"

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define LEDTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_LED_TASK       2

/* Converts a time in ticks to a time in milliseconds. */
#define pdTICKS_TO_MS( xTimeInTicks ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInTicks ) * ( TickType_t ) 1000 ) / ( TickType_t ) configTICK_RATE_HZ ) )

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the UART.
//
//*****************************************************************************
uint32_t g_ui32Flags;

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

/* Semaphore to be used to wake up LED task at 10 Hz */
xSemaphoreHandle xSemaphoreLed;

/* Timer handler */
void Timer0IntHandler(void)
{
    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//    unsigned int currTime = xTaskGetTickCount();
//    UARTprintf("Current handler: %d\n", pdTICKS_TO_MS(currTime));
//    UARTprintf("Interrupt Giving LED Semaphore\n");
    xSemaphoreGive(xSemaphoreLed);
}

//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency.
//
//*****************************************************************************
static void
LEDTask(void *pvParameters)
{
        static int count;               // Toggle count
        uint32_t currTime;              // Timestamp
        LED_t ledUpdate;
        QUEUE_t queueUpdate;

        //
        // Loop forever.
        //
        while(1)
        {
//            UARTprintf("Waiting for LED Semaphore\n");

            xSemaphoreTake(xSemaphoreLed, portMAX_DELAY);

            currTime = xTaskGetTickCount();
//            UARTprintf("Current time: %d\n", pdTICKS_TO_MS(currTime));

            //
            // Toggle the flag for the first LED
            //
            HWREGBITW(&g_ui32Flags, 0) ^= 1;

            //
            // Use the flags to Toggle the first LED
            //
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, g_ui32Flags);

            //
            // Toggle the flag for the second LED
            //
            HWREGBITW(&g_ui32Flags, 1) ^= 1;

            //
            // Use the flags to Toggle the second LED
            //
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, g_ui32Flags);

            count++;

            //
            // Update the led structure
            //
            ledUpdate.tCount = count;
            ledUpdate.timeNow = pdTICKS_TO_MS(currTime);
            strcpy(ledUpdate.name,"Smitesh");

            queueUpdate.taskID = LED_TASK;
            queueUpdate.TASK_t.led_task_t = ledUpdate;

            enqueueForLog(&queueUpdate);
        }
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
LEDTaskInit(void)
{
    //
    // Enable the GPIO port that is used for the on-board LEDs.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    //
    // Enable the GPIO pins for the LEDs (PN0 & PN1).
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Enable the Timer peripheral.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Configure the two 32-bit periodic timer.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A,  g_ui32SysClock/10 - 1);

    //
    // Setup the interrupts for the timer timeout.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timer.
    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    // Create a Semaphore to signal the task
    xSemaphoreLed = xSemaphoreCreateBinary();

    //
    // Create the LED task.
    //
    if(xTaskCreate(LEDTask, (const portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
