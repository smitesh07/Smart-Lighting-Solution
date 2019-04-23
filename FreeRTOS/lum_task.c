/*
 * lum_task.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#include "lum_task.h"

//*****************************************************************************
//
// The stack size for the Luminosity sensor task.
//
//*****************************************************************************
#define LUMTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_LUM_TASK       3

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

/* Semaphore to be used to wake up Temp task at 1 Hz */
xSemaphoreHandle xSemaphoreLum;

//*****************************************************************************
//
// Timer handler.
//
//*****************************************************************************
void Timer0IntHandler(void) {
    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//    unsigned int currTime = xTaskGetTickCount();
//    UARTprintf("Current handler: %d\n", pdTICKS_TO_MS(currTime));
    UARTprintf("Interrupt Giving Lum Semaphore\n");
    xSemaphoreGive(xSemaphoreLum);
}

//*****************************************************************************
//
// Luminosity sensor task.
//
//*****************************************************************************
void lumTask( void *pvParameters ) {
    int_fast32_t i32IntegerPart;
    int_fast32_t i32FractionPart;
    while(1) {
        xSemaphoreTake(xSemaphoreLum, portMAX_DELAY);
        float lum = getLum();
        i32IntegerPart = (int32_t)lum;
        i32FractionPart = (int32_t)(lum * 1000.0f);
        i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
        if(i32FractionPart < 0)
        {
            i32FractionPart *= -1;
        }
//        UARTprintf("\n[%d ms]       Temperature Value: %3d.%03d\n\n",
//                   pxRxedMessage->TASK_t.temp_task_t.timeNow, i32IntegerPart, i32FractionPart);
        UARTprintf("LUM VALUE: %3d.%03d\n", i32IntegerPart, i32FractionPart);
//        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//*****************************************************************************
//
// Initializes the Luminosity sensor task.
//
//*****************************************************************************
uint32_t lumTaskInit(void) {

    I2CInit();

    //
    // Enable the Timer peripheral.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //
    // Configure the two 32-bit periodic timer.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A,  g_ui32SysClock - 1);

    //
    // Setup the interrupts for the timer timeout.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timer.
    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    // Create a Semaphore to signal the task
    xSemaphoreLum = xSemaphoreCreateBinary();


    initLumSensor();

    //
    // Create the lum task.
    //
    if(xTaskCreate(lumTask, (const portCHAR *)"QUEUE", LUMTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LUM_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
