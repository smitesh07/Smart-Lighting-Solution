/*
 * log_task.c
 *
 *  Created on: Apr 8, 2019
 *      Author: modak
 */

#include "log_task.h"
#include <stdio.h>

//*****************************************************************************
//
// The stack size for the LOG task.
//
//*****************************************************************************
#define LOGTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_LOG_TASK       3

QueueHandle_t xQueue;

// Function to post a value.
 void enqueueForLog( QUEUE_t *xMessage) {
     QUEUE_t *pxMessage;

    // Send a pointer to a struct QUEUE_t.  Don't block if the
    // queue is already full.
    pxMessage = xMessage;
    xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
 }

 // Task to receive from the queue and log it.
 void DequeueFromLogTask( void *pvParameters )
  {
     QUEUE_t *pxRxedMessage;
     int_fast32_t i32IntegerPart;
     int_fast32_t i32FractionPart;

     while(1) {
         if( xQueue != 0 )
         {
             // Receive a message on the created queue.  Block for 10 ticks if a
             // message is not immediately available.
             if( xQueueReceive( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
             {
                 // pcRxedMessage now points to the struct LED_t posted
                 // by vATask.
                 if (pxRxedMessage->taskID == LED_TASK) {
                     UARTprintf("[%d ms]       Toggle Count: %d,       Name: %s\n",
                                pxRxedMessage->TASK_t.led_task_t.timeNow, pxRxedMessage->TASK_t.led_task_t.tCount, pxRxedMessage->TASK_t.led_task_t.name);
                 } else if (pxRxedMessage->taskID == TEMP_TASK) {
                     i32IntegerPart = (int32_t)pxRxedMessage->TASK_t.temp_task_t.tempVal;
                     i32FractionPart = (int32_t)(pxRxedMessage->TASK_t.temp_task_t.tempVal * 1000.0f);
                     i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
                     if(i32FractionPart < 0)
                     {
                         i32FractionPart *= -1;
                     }
                     UARTprintf("\n[%d ms]       Temperature Value: %3d.%03d\n\n",
                                pxRxedMessage->TASK_t.temp_task_t.timeNow, i32IntegerPart, i32FractionPart);
                 }
             }
         }
     }
 }

 //*****************************************************************************
 //
 // Initializes the Log task.
 //
 //*****************************************************************************
 uint32_t
 LogTaskInit(void)
 {

     // Create a queue capable of containing 10 pointers to QUEUE_t structures.
     // These should be passed by pointer as they contain a lot of data.
     xQueue = xQueueCreate( 10, sizeof( QUEUE_t * ) );

     //
     // Create the Log task.
     //
     if(xTaskCreate(DequeueFromLogTask, (const portCHAR *)"QUEUE", LOGTASKSTACKSIZE, NULL,
                    tskIDLE_PRIORITY + PRIORITY_LOG_TASK, NULL) != pdTRUE)
     {
         return(1);
     }

     //
     // Success.
     //
     return(0);
 }
