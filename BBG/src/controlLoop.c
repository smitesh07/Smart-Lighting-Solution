/**
 * @file controlLoop.c
 * @author Smitesh Modak and Ashish Tak
 * @brief 
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "controlLoop.h"

bool controlHeartbeatFlag;

void *controlLoopHandler(void *arg) {
    while (1) {
      deQueueFromLog();
      fflush(filePtr);
      //Periodically set the heartbeat flag to be checked by main()
      controlHeartbeatFlag=true;
      //Main sets this global flag on receiving the SIGINT signal from user
      if (terminateSignal) {
        enQueueForLog(WARN, "Termination signal received to Control Loop thread.", 0);
        deQueueFromLog();
        fflush(filePtr);
        break;
      }
      sleep(1);
    }
}