/*
 * my_i2c.h
 *
 *  Created on: Apr 10, 2019
 *      Author: modak
 *      Reference: CU Boulder ECEN 5013 Class Demo
 */

#ifndef MY_I2C_H_
#define MY_I2C_H_

#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"

#define BUSY_WAIT 1

void I2CSendByte(uint8_t target_address, uint8_t register_address, uint8_t data);
uint8_t I2CGetByte(uint8_t target_address, uint8_t register_address);
void I2CGet2Bytes(uint8_t *buf, uint8_t target_address, uint8_t register_address);

#endif /* MY_I2C_H_ */
