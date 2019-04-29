/**
 * @file main.c
 * @author Smitesh Modak and Ashish Tak
 * @brief
 * @version 0.1
 * @date 2019-04-27
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <mqueue.h>
#include <signal.h>
#include "log.h"
#include "timer.h"
#include "queue.h"
#include "uart.h"
#include "controlLoop.h"


#define HEARTBEAT_TIMEOUT 10 //in seconds
#define FILENAME_MAX_LEN 50

pthread_t logger, uart, controlLoop;

//Global flag to be set by main on receiving the SIGINT signal
bool terminateSignal= false;

timer_t mainTimerid;

/**
 * @brief Timer callback function to check (and reset) the heartbeat flags from individual threads
 *
 */
void heartbeatTimerHandler () {
    if (uartHeartbeatFlag)
        uartHeartbeatFlag = false;
    else {
        enQueueForLog(ERROR, "Uart thread is DEAD!! Issuing pthread_cancel().. ", 0);
        deQueueFromLog();
        fflush(filePtr);
        pthread_cancel(uart);    
    }

    if (controlHeartbeatFlag)
        controlHeartbeatFlag = false;
    else {
        enQueueForLog(ERROR, "Control loop thread is DEAD!! Issuing pthread_cancel().. ", 0);
        deQueueFromLog();
        fflush(filePtr);
        pthread_cancel(controlLoop);    
    }
    

    if (logHeartbeatFlag)
        logHeartbeatFlag=false;
    else {
        enQueueForLog(ERROR, "Logger thread is DEAD!! Issuing pthread_cancel().. ", 0);
        deQueueFromLog();
        fflush(filePtr);
        pthread_cancel(logger);
    }

    return;
}


/**
 * @brief Signal handler for the SIGINT signal
 *
 * @param signal
 */
void sigHandler (int signal) {
	switch (signal) {
		case SIGINT:
            //Set the flag to give a termination signal to all threads.
            //Since the flag is written to only once in the lifetime of
            //the program, no synchronization is required
			terminateSignal=true;
            //Explictly send a signal to the External Socket handling thread,
            //since its blocking on external requests and does not periodically
            //monitor any termination signal / flag
            // pthread_kill(externSocket, SIGUSR1);
			break;
		default:
			break;
	}
}


/**
 * @brief Main
 *
 */
int main(int argc, char *argv[])
{
    char logFile[FILENAME_MAX_LEN];
    char fileName[FILENAME_MAX_LEN];

    if (argc > 1) {
      strcpy(logFile, argv[1]);
      strcpy(fileName, argv[2]);
      strcat(logFile,fileName);
    } 
    else {
        printf("\nNo Log file name provided. Will use the default 'log.txt\n");
        strcpy(logFile, "log.txt");
    }

    char path[] = "/OpenTest_MQ";
    logInit(logFile);
    initQueue(path);

    //Register the signal handler for the termination signal (SIGINT) from the user
    struct sigaction sa;
	sigemptyset (&sa.sa_mask);
	sa.sa_handler=&sigHandler;
	sa.sa_flags=0;
	sigaction(SIGINT, &sa, NULL);

    pthread_create (&uart, NULL, uartHandler, NULL);
    pthread_create (&controlLoop, NULL, controlLoopHandler, NULL);
    pthread_create (&logger, NULL, loggerHandler, NULL);

    mainTimerid= initTimer(HEARTBEAT_TIMEOUT*(uint64_t)1000000000, heartbeatTimerHandler);

    pthread_join(uart, NULL);
    pthread_join(controlLoop, NULL);
    pthread_join(logger, NULL);

    enQueueForLog(WARN, "Main thread terminating..", 0);
    deQueueFromLog();
    fflush(filePtr);
    logFlush();

    //Cleanup procedure for main()
    timer_delete(mainTimerid);
    mq_close(mqdes);
    mq_unlink (path);

    return 0;
}