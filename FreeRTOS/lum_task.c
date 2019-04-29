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
#define LUMTASKSTACKSIZE    128         // Stack size in words

#define PRIORITY_LUM_TASK   4

#define LUM_TASK_PERIOD     5000        //in ms

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

/* Semaphore to be used to wake up Lum task at 1 Hz */
xSemaphoreHandle xSemaphoreLum;

/* Handle to the created timer. */
 TimerHandle_t xLumTimer;

extern xSemaphoreHandle UARTTxDataSem;
extern sensorTx dataOut;


/* Define a callback function that will be used by Lum timer
 instance.  The callback function does nothing but pass on the semaphore to the waiting task */
 void vLumTimerCallback( TimerHandle_t xTimer )
 {
//     UARTprintf("Interrupt Giving Lum Semaphore\n");
     xSemaphoreGive(xSemaphoreLum);
 }

//*****************************************************************************
//
// Initialize and enable timer to signal task after timeout.
//
//*****************************************************************************
static void enableTaskTimer(void) {
    xLumTimer = xTimerCreate
                       ( /* Just a text name, not used by the RTOS
                         kernel. */
                         "Timer",
                         /* The timer period in ticks, must be
                         greater than 0. */
                         pdMS_TO_TICKS(LUM_TASK_PERIOD),
                         /* The timers will auto-reload themselves
                         when they expire. */
                         pdTRUE,
                         /* The ID is used to store a count of the
                         number of times the timer has expired, which
                         is initialised to 0. */
                         ( void * ) 0,
                         /* Each timer calls the same callback when
                         it expires. */
                         vLumTimerCallback
                       );

    if( xLumTimer == NULL )
    {
        /* The timer was not created. */
        UARTprintf("Lum Timer not created \n");
    }
    else
    {
        UARTprintf("Lum Timer created \n");
        /* Start the timer.  No block time is specified, and
                 even if one was it would be ignored because the RTOS
                 scheduler has not yet been started. */
        if( xTimerStart( xLumTimer, 0 ) != pdPASS )
        {
            /* The timer could not be set into the Active
                     state. */
        }
    }

}

//*****************************************************************************
//
// Luminosity sensor task.
//
//*****************************************************************************
static void lumTask( void *pvParameters ) {
    int_fast32_t i32IntegerPart;
    int_fast32_t i32FractionPart;
    while(1) {
        xSemaphoreTake(xSemaphoreLum, portMAX_DELAY);
//        UARTprintf("Lum task initiated\n");
        float lux = getLum();

        xSemaphoreTake(UARTTxDataSem, portMAX_DELAY);
        if (lux<0) {
            dataOut.sensorStatus = SENSOR_NOT_WORKING;
            dataOut.lux=0;
        }
        else {
            dataOut.sensorStatus = SENSOR_WORKING;
            dataOut.lux = lux;
        }
        xSemaphoreGive(UARTTxDataSem);

        i32IntegerPart = (int32_t)lux;
        i32FractionPart = (int32_t)(lux * 1000.0f);
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

    // Initialize I2C
    I2CInit();

    // Enable task timer
    enableTaskTimer();

    // Create a Semaphore to signal the task
    xSemaphoreLum = xSemaphoreCreateBinary();

    // Initialize luminosity sensor
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
