/*
 * prox_task.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#include "prox_task.h"

//*****************************************************************************
//
// The stack size for the Proximity Sensor task.
//
//*****************************************************************************
#define PROXTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_PROX_TASK       3

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// Proximity sensor task.
//
//*****************************************************************************
void proxTask( void *pvParameters ) {

}

//*****************************************************************************
//
// Initializes the Proximity sensor task.
//
//*****************************************************************************
uint32_t proxTaskInit(void) {
    //
    // Create the prox task.
    //
    if(xTaskCreate(proxTask, (const portCHAR *)"QUEUE", PROXTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_PROX_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
