/**
 * @file log.c
 * @author Smitesh Modak and Ashish Tak
 * @brief log source file that initializes log file and writes out to log file
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "log.h"
#include "queue.h"

FILE *filePtr;
bool logHeartbeatFlag;

uint32_t loggerGetTimestamp(void) {
  /* Current date and time */
  struct timeval cur_tv;
  struct timezone *cur_tz = NULL;
  gettimeofday(&cur_tv,cur_tz);
  uint32_t timeStamp = (cur_tv.tv_sec/1000000)+cur_tv.tv_usec;

  return timeStamp;
}

void logInit(char *logFile) {
    char fileName[10];
    strcpy(fileName, logFile);
    filePtr = fopen((char *)fileName,"w");
}

void logFlush(void) { 
  fflush(filePtr);
  fclose(filePtr);
}


void *loggerHandler(void *arg) {
    while (1) {
      deQueueFromLog();
      fflush(filePtr);
      //Periodically set the heartbeat flag to be checked by main()
      logHeartbeatFlag=true;
      //Main sets this global flag on receiving the SIGINT signal from user
      if (terminateSignal) {
        enQueueForLog(WARN, "Termination signal received to Logger thread.", 0);
        break;
      }
      sleep(1);
    }
}