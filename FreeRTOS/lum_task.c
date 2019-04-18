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

//*****************************************************************************
//
// Luminosity sensor task.
//
//*****************************************************************************
void lumTask( void *pvParameters ) {

}

//*****************************************************************************
//
// Initializes the Luminosity sensor task.
//
//*****************************************************************************
uint32_t lumTaskInit(void) {
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
