/**
 * @file apds9301.c
 * @author Smitesh Modak and Ashish Tak
 * @brief : Implementation of the library/HAL of the APDS-9301 Luminosity sensor
 * @version 0.1
 * @date 2019-03-30
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef SRC_APDS9301_H_
#define SRC_APDS9301_H_

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

/**
 * @brief Perform the initialization routine for the luminosity sensor
 *
 * @return int : Success/Failure (0/-1) of the initialization operation
 */
int initLumSensor(void);

/**
 * @brief Get the current Luminosity value from the sensor
 *
 * @return float: Luminosity value
 */
float getLum(void);

//float readTemp(void);

#endif /* SRC_APDS9301_H_ */
