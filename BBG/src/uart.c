/**
 * @file uart.c
 * @author Smitesh Modak and Ashish Tak
 * @brief  
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "uart.h"

//Path to the file for using UART1
#define UART_DEV_FILE "/dev/ttyO1"

//Transmission and reception intervals in seconds
#define UART_TRANSMISSION_INTERVAL 5
#define UART_RECEPTION_INTERVAL 5

bool uartHeartbeatFlag;
timer_t uartTxTimerid, uartRxTimerid;
int fd;
CONTROL_RX_t dataIn;
CONTROL_TX_t dataOut;


/**
 * @brief Function to perform the UART Initialization routine
 * 
 */
void uartInit (void) {
    char buf[] = UART_DEV_FILE;
    struct termios uart1;
	  fd = open(buf, O_RDWR | O_NOCTTY );
	  if(fd < 0) printf("port failed to open\n");

    bzero(&uart1,sizeof(uart1));

    //Set the UART parameters: Baud 115200, 8-bit mode, Ignore parity
    uart1.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    uart1.c_iflag = IGNPAR | ICRNL;
    uart1.c_oflag = 0;
    uart1.c_lflag = 0;

    uart1.c_cc[VTIME] = 0;
    //Wait for the specified number of bytes for read() to complete
    uart1.c_cc[VMIN] = sizeof(CONTROL_RX_t);

    //clean the line and set the attributes
    tcflush(fd,TCIOFLUSH);
    tcsetattr(fd,TCSANOW,&uart1);
}


/**
 * @brief Timer ISR to trigger UART transmission to the Tiva board
 * 
 */
void UARTTransmissionTrigger (void) {
    //TODO: Integrate with the Control loop
    dataOut.light=1;
    dataOut.motor=0;  
    tcflush(fd,TCOFLUSH);
    write(fd, &dataOut, sizeof(CONTROL_TX_t));
}


/**
 * @brief Timer ISR to trigger UART reception from the Tiva board
 * 
 */
void UARTReceptionTrigger (void) {
    read(fd, &dataIn, sizeof(CONTROL_RX_t));
    printf("\nRecd values: %f\t%d\t%d\t%d",dataIn.lux, dataIn.proximity, dataIn.sensorStatus, dataIn.blindsStatus);
    tcflush(fd,TCIFLUSH);
}


void *uartHandler(void *arg) {
    uartInit();

    uartTxTimerid= initTimer(UART_TRANSMISSION_INTERVAL*(uint64_t)1000000000, UARTTransmissionTrigger);
    uartRxTimerid= initTimer(UART_RECEPTION_INTERVAL*(uint64_t)1000000000, UARTReceptionTrigger);

    while (1) {
      deQueueFromLog();
      fflush(filePtr);
      //Periodically set the heartbeat flag to be checked by main()
      uartHeartbeatFlag=true;
      //Main sets this global flag on receiving the SIGINT signal from user
      if (terminateSignal) {
        enQueueForLog(WARN, "Termination signal received to UART thread.", 0);
        deQueueFromLog();
        fflush(filePtr);
        break;
      }
      sleep(1);
    }
}