/*
 * my_i2c.c
 *
 *  Created on: Apr 10, 2019
 *      Author: modak
 *      Reference: CU Boulder ECEN 5013 Class Demo
 */

#include "my_i2c.h"

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

void I2CInit(void) {
    //
    // Enable the I2C2 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);

    //
    // Enable the GPIO port that is used for I2C.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Configure the I2CSDA and I2CSCL pins
    //
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinConfigure(GPIO_PN5_I2C2SCL);

    //
    // Select the I2C function for these pins.
    //
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);

    //
    // Enable and initialize the I2C2 master module.
    //
    I2CMasterInitExpClk(I2C2_BASE, g_ui32SysClock, false);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();
}

int I2CSendByte(uint8_t target_address, uint8_t register_address, uint8_t data)
{
    unsigned int count = 0;
   //
   // Tell the master module what address it will place on the bus when
   // communicating with the slave.  Set the address to TEMP_SENSOR
   // (as set in the slave module).  The receive parameter is set to false
   // which indicates the I2C Master is initiating a writes to the slave.  If
   // true, that would indicate that the I2C Master is initiating reads from
   // the slave.
   //
   I2CMasterSlaveAddrSet(I2C2_BASE, target_address, false);

   //
   // Place the data to be sent in the data register
   //
   I2CMasterDataPut(I2C2_BASE, register_address);

   //
   // Initiate send of data from the master.
   //
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE) && count < TIMEOUT){
       count ++;
   }
   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }
#endif

   /*
   ** now wait for it to be non-busy
   */
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT){
       count ++;
   }

   if (count > TIMEOUT) {
          count = 0;
          return -1;
   }

   //
   // Place the data to be sent in the data register
   //
   I2CMasterDataPut(I2C2_BASE, data);

   //
   // Initiate send of data from the master.
   //
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE)  && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }
#endif
   //
   // Wait until master module is done
   //
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }

   return 0;
}

uint8_t I2CGetByte(uint8_t target_address, uint8_t register_address)
{
    unsigned int count = 0;
   //
   // Tell the master module what address it will place on the bus when
   // communicating with the slave.  Set the address to TEMP_SENSOR
   // (as set in the slave module).  The receive parameter is set to false
   // which indicates the I2C Master is initiating a writes to the slave.  If
   // true, that would indicate that the I2C Master is initiating reads from
   // the slave.
   //
   I2CMasterSlaveAddrSet(I2C2_BASE, target_address, false);

   //
   // Place the data to be sent in the data register
   //
   I2CMasterDataPut(I2C2_BASE, register_address);

   //
   // Initiate send of data from the master.  Since the loopback
   // mode is enabled, the master and slave units are connected
   // allowing us to receive the same data that we sent out.
   //
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return 255;
   }
#endif

   //
   // Wait until master module is done
   //
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return 255;
   }

   /*
   ** now switch to read mode
   */
   I2CMasterSlaveAddrSet(I2C2_BASE, target_address, true);

   /*
   ** read one byte
   */
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return 255;
   }
#endif

   //
   // Wait until master module is done
   //
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return 255;
   }

   return I2CMasterDataGet(I2C2_BASE);
}


int I2CGet2Bytes(uint8_t target_address, uint8_t register_address, uint8_t *buf)
{
    unsigned int count = 0;
   //
   // Tell the master module what address it will place on the bus when
   // communicating with the slave.  Set the address to TEMP_SENSOR
   // (as set in the slave module).  The receive parameter is set to false
   // which indicates the I2C Master is initiating a writes to the slave.  If
   // true, that would indicate that the I2C Master is initiating reads from
   // the slave.
   //
   I2CMasterSlaveAddrSet(I2C2_BASE, target_address, false);

   //
   // Place the data to be sent in the data register
   //
   I2CMasterDataPut(I2C2_BASE, register_address);

   //
   // Initiate send of data from the master.  Since the loopback
   // mode is enabled, the master and slave units are connected
   // allowing us to receive the same data that we sent out.
   //
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }
#endif

   //
   // Wait until master module is done
   //
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }

   /*
   ** now switch to read mode
   */
   I2CMasterSlaveAddrSet(I2C2_BASE, target_address, true);

   /*
   ** read one byte
   */
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }
#endif

   //
   // Wait until master module is done
   //
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }

   uint32_t data_one = I2CMasterDataGet(I2C2_BASE);

   /*
   ** read one byte
   */
   I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

#ifdef BUSY_WAIT
   //
   // Wait until master module is says it's busy. Errata I2C#08
   //
   while(!I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }
#endif

   //
   // Wait until master module is done
   //
   while(I2CMasterBusy(I2C2_BASE) && count < TIMEOUT) {
       count ++;
   }

   if (count > TIMEOUT) {
       count = 0;
       return -1;
   }

   uint32_t data_two = I2CMasterDataGet(I2C2_BASE);

   buf[0] = data_one;
   buf[1] = data_two;

   return 0;
}


