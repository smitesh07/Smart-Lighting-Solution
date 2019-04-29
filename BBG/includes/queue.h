/**
 * @file queue.h
 * @author Smitesh Modak and Ashish Tak
 * @brief queue header file containing structures and function prototypes
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include "log.h"
#include "controlLoop.h"

typedef enum {
  CONTROL_TX,
  CONTROL_RX,
  PLAIN_MSG
} MESSAGE_TYPE;

/**
 * @brief message queue structure
 * 
 */
typedef struct {
  long mtype;
  LOG_t logQueue;
}QUEUE_t;

extern mqd_t mqdes;

/**
 * @brief 
 * 
 * @param queueName   message queue name
 */
void initQueue(char *queueName);

/**
 * @brief enqueue to the logger queue
 * 
 * @param level   level/priority of message
 * @param msg     message
 * @param value   value from the sensors
 */
void enQueueForLog(MESSAGE_TYPE msgtype, LOG_LEVEL_t level, char *msg, CONTROL_RX_t *rxControl, CONTROL_TX_t *txControl);

/**
 * @brief dequeue from the logger queue and LOG it
 * 
 */
void deQueueFromLog(void);

#endif