/*
 * alert_task.c
 *
 *  Created on: Apr 10, 2019
 *      Author: modak
 */

#include "alert_task.h"

//*****************************************************************************
//
// The stack size for the Alert task.
//
//*****************************************************************************
#define ALERTTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_ALERT_TASK       tskIDLE_PRIORITY

/* Converts a time in ticks to a time in milliseconds. */
#define pdTICKS_TO_MS( xTimeInTicks ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInTicks ) * ( TickType_t ) 1000 ) / ( TickType_t ) configTICK_RATE_HZ ) )

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

TaskHandle_t xAlertTask = NULL;

//*****************************************************************************
//
// This task waits to be notified by the Temperature sensor task whenever
// the temperature value crosses user defined threshold.
//
//*****************************************************************************
static void
AlertTask(void *pvParameters)
{
    uint32_t currTime;              // Timestamp

    for( ;; )
    {
        /* Block to wait to notify this task. */
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        currTime = xTaskGetTickCount();
        UARTprintf("[%d ms]       ALERT!!!!!!!!! \n\n",pdTICKS_TO_MS(currTime));
    }
}

//*****************************************************************************
//
// Initializes the Alert task.
//
//*****************************************************************************
uint32_t
AlertTaskInit(void)
{
    //
    // Create the Alert task.
    //
    if(xTaskCreate(AlertTask, (const portCHAR *)"LED", ALERTTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_ALERT_TASK, &xAlertTask) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}


