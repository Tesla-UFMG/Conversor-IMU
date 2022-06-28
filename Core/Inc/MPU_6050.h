/*
 * MPU_6050.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_MPU_6050_H_
#define INC_MPU_6050_H_

#include "main.h"

extern HAL_StatusTypeDef MPU_6050_receive_accelerometer(int16_t* accelerometer);

extern HAL_StatusTypeDef MPU_6050_receive_gyroscope(int16_t* gyroscope);

extern HAL_StatusTypeDef MPU_6050_receive_temperature(uint16_t* temperature);

extern HAL_StatusTypeDef MPU_6050_request_temperature();

extern HAL_StatusTypeDef MPU_6050_request_accelerometer();

extern HAL_StatusTypeDef MPU_6050_request_gyroscope();

extern HAL_StatusTypeDef MPU_6050_send_command(uint8_t buffer);

extern HAL_StatusTypeDef MPU_6050_read_data(uint8_t* buffer, uint16_t buffer_size);

#endif /* INC_MPU_6050_H_ */
