/*
 * IMU.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "MPU_6050.h"
#include "stm32f1xx_hal.h"

HAL_StatusTypeDef IMU_initialize(void);

HAL_StatusTypeDef get_accelerometer_value(accelerometer_t* accelerometer);

HAL_StatusTypeDef get_gyroscope_value(gyroscope_t* gyroscope);

HAL_StatusTypeDef get_temperature_value(int16_t* temperature);

#endif /* INC_IMU_H_ */
