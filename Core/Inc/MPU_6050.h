/*
 * MPU_6050.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_MPU_6050_H_
#define INC_MPU_6050_H_

HAL_StatusTypeDef MPU_6050_receive_accelerometer(uint8_t* accelerometer);

HAL_StatusTypeDef MPU_6050_receive_gyroscope(uint8_t* gyroscope);

HAL_StatusTypeDef MPU_6050_request_accelerometer();

HAL_StatusTypeDef MPU_6050_request_gyroscope();

HAL_StatusTypeDef MPU_6050_send_command(uint8_t buffer);

HAL_StatusTypeDef MPU_6050_read_data(uint8_t* buffer);

#endif /* INC_MPU_6050_H_ */
