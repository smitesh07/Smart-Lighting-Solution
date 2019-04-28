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

// default state of the light task on Tiva
LIGHT_CONTROL lightControl = LIGHT_NO_CHANGE;

// default state of the motor task on Tiva
MOTOR_CONTROL motorControl = MOTOR_NO_CHANGE;

void getCurrentAction (void) {
  CONTROL_TX_t *controlAction;
  controlAction = (CONTROL_TX_t *) malloc (sizeof(CONTROL_TX_t));

  float lum;
  uint8_t proximity, sensorStatus, blindStatus;

  // TODO: request current luminosity value, proximity, sensorstatus and blindstatus

  if (sensorStatus == 0) {
    // TODO: return ERROR condition on TIVA
    controlAction->light = LIGHT_MAINTAIN_DEFAULT;
    controlAction->motor = MOTOR_NO_CHANGE;
  } else if(proximity == 1) {
    if(lum > LOW_LIGHT && lum < HIGH_LIGHT) {
      controlAction->light = LIGHT_NO_CHANGE;
      controlAction->motor = MOTOR_NO_CHANGE;
    } else if (lum < LOW_LIGHT && blindStatus == MOTOR_CLOSE) {
      controlAction->light = LIGHT_NO_CHANGE;
      controlAction->motor = MOTOR_OPEN;
    } else if (lum < LOW_LIGHT && blindStatus == MOTOR_OPEN) {
      controlAction->light = LIGHT_INCREASE;
      controlAction->motor = MOTOR_CLOSE;
    } else if (lum > HIGH_LIGHT && blindStatus == MOTOR_OPEN) {
      controlAction->light = LIGHT_NO_CHANGE;
      controlAction->motor = MOTOR_CLOSE;
    } else if (lum > HIGH_LIGHT && blindStatus == MOTOR_CLOSE) {
      controlAction->light = LIGHT_DECREASE;
      controlAction->motor = MOTOR_NO_CHANGE;
    } else {
    controlAction->light = LIGHT_NO_CHANGE;
    controlAction->motor = MOTOR_NO_CHANGE;
    }
  } 

  // TODO: Transmit the control action using uart 
  
}

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