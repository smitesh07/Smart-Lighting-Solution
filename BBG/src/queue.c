/**
 * @file queue.c
 * @author Smitesh Modak and Ashish Tak
 * @brief queue source file that initializes queue, enqueues and dequeues message queue
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "queue.h"

// To store queue attributes
struct mq_attr attr,old_attr;

// Message queue descriptors
mqd_t mqdes;

void initQueue(char *queueName) {
    // First we need to set up the attribute structure
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(QUEUE_t);
    attr.mq_flags = 0;

    char path[50];
    strcpy(path,queueName);
    // printf("Path %s", path);

    mq_unlink(queueName);
    
    // Open a queue with the attribute structure
    mqdes = mq_open (queueName, O_RDWR | O_CREAT, 0666, &attr);
    if (mqdes <0) {
        perror("mq_open()");
        enQueueForLog(ERROR, "Failed to open message queue", 0);
    }
}

void enQueueForLog(MESSAGE_TYPE msgtype, LOG_LEVEL_t level, char *msg, CONTROL_RX_t *rxControlLog, CONTROL_TX_t *txControlLog) {
    /* pointer to priority queue */
    QUEUE_t *prioQueue;
    prioQueue = (QUEUE_t *)malloc(sizeof(QUEUE_t));

    CONTROL_RX_t rxControl;
    CONTROL_TX_t txControl;

    LOG_LEVEL_t prio;
    
    prio = 0;

    prioQueue->mtype = msgtype;
    (prioQueue->logQueue).level = level;
    strcpy((prioQueue->logQueue).msg, msg);

    // if (msgtype == PLAIN_MSG) {
    //     (prioQueue->logQueue).controlRx = 0;
    //     (prioQueue->logQueue).controlTx = 0;
    // } else 
    if (msgtype == CONTROL_RX) {
        rxControl.lux = rxControlLog->lux;
        rxControl.blindsStatus = rxControlLog->proximity;
        rxControl.sensorStatus = rxControlLog->sensorStatus;
        rxControl.blindsStatus = rxControlLog->blindsStatus;
        (prioQueue->logQueue).controlRx = rxControl;
    } else if (msgtype == CONTROL_TX) {
        txControl.light = txControlLog->light;
        txControl.motor = txControlLog->motor;
        (prioQueue->logQueue).controlTx = txControl;
    }
       
    if (mq_send (mqdes, (const char *)prioQueue, sizeof(QUEUE_t), prio)< 0) {
        perror ("mq_send()");
        enQueueForLog(ERROR, "Failed writing to message queue",0);
    }

    free(prioQueue);   
}

void deQueueFromLog(void) {
     /* pointer to priority queue */
    QUEUE_t *prioQueue;
    prioQueue = (QUEUE_t *)malloc(sizeof(QUEUE_t));
    LOG_LEVEL_t prio;
    // printf("Current messages %d", attr.mq_curmsgs);
    // if (attr.mq_curmsgs > 0) {
        // printf("After");
        // There are some messages on this queue....eat em
        
        // First set the queue to not block any calls    
        attr.mq_flags = O_NONBLOCK;
        mq_setattr (mqdes, &attr, &old_attr);  
            
        // Now eat all of the messages
        while (mq_receive (mqdes, (char *)prioQueue, sizeof(QUEUE_t), &prio) != -1) {
            if(prioQueue->mtype == PLAIN_MSG) {
                if((prioQueue->logQueue).level == ERROR)
                    LOG_ERROR("%s",(prioQueue->logQueue).msg);
                else if ((prioQueue->logQueue).level == WARN)
                    LOG_WARN("%s",(prioQueue->logQueue).msg); 
                else if ((prioQueue->logQueue).level == INFO)
                    LOG_INFO("%s",(prioQueue->logQueue).msg);
                else if ((prioQueue->logQueue).level == DEBUG)
                    LOG_DEBUG("%s",(prioQueue->logQueue).msg);
            } else if (prioQueue->mtype = CONTROL_TX) {
                if((prioQueue->logQueue).level == ERROR)
                    LOG_ERROR("%s LUX: %f   Proximity: %d   Sensor Status: %d   Blind Status: %d   ",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlRx.lux,
                    (prioQueue->logQueue).controlRx.proximity,
                    (prioQueue->logQueue).controlRx.sensorStatus,
                    (prioQueue->logQueue).controlRx.blindsStatus);
                else if ((prioQueue->logQueue).level == WARN)
                    LOG_WARN("%s LUX: %f   Proximity: %d   Sensor Status: %d   Blind Status: %d   ",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlRx.lux,
                    (prioQueue->logQueue).controlRx.proximity,
                    (prioQueue->logQueue).controlRx.sensorStatus,
                    (prioQueue->logQueue).controlRx.blindsStatus); 
                else if ((prioQueue->logQueue).level == INFO)
                    LOG_INFO("%s LUX: %f   Proximity: %d   Sensor Status: %d   Blind Status: %d   ",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlRx.lux,
                    (prioQueue->logQueue).controlRx.proximity,
                    (prioQueue->logQueue).controlRx.sensorStatus,
                    (prioQueue->logQueue).controlRx.blindsStatus);
                else if ((prioQueue->logQueue).level == DEBUG)
                    LOG_DEBUG("%s LUX: %f   Proximity: %d   Sensor Status: %d   Blind Status: %d   ",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlRx.lux,
                    (prioQueue->logQueue).controlRx.proximity,
                    (prioQueue->logQueue).controlRx.sensorStatus,
                    (prioQueue->logQueue).controlRx.blindsStatus);

            } else if (prioQueue->mtype = CONTROL_RX) {
                if((prioQueue->logQueue).level == ERROR)
                    LOG_ERROR("%s Light control: %d   Motor control: %d",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlTx.light,
                    (prioQueue->logQueue).controlTx.motor);
                else if ((prioQueue->logQueue).level == WARN)
                    LOG_WARN("%s Light control: %d   Motor control: %d",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlTx.light,
                    (prioQueue->logQueue).controlTx.motor); 
                else if ((prioQueue->logQueue).level == INFO)
                    LOG_INFO("%s Light control: %d   Motor control: %d",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlTx.light,
                    (prioQueue->logQueue).controlTx.motor);
                else if ((prioQueue->logQueue).level == DEBUG)
                    LOG_DEBUG("%s Light control: %d   Motor control: %d",
                    (prioQueue->logQueue).msg, (prioQueue->logQueue).controlTx.light,
                    (prioQueue->logQueue).controlTx.motor);

            }
        }

        
        fflush(filePtr);
        
        free(prioQueue);
        
        // Now restore the attributes
        mq_setattr (mqdes, &old_attr, 0);
}