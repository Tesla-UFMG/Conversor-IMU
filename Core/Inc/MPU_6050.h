/*
 * MPU_6050.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_MPU_6050_H_
#define INC_MPU_6050_H_

#include "stm32f1xx_hal.h"

typedef enum {
    FILTER_260HZ = 0,
    FILTER_184HZ,
    FILTER_94HZ,
    FILTER_44HZ,
    FILTER_21HZ,
    FILTER_10HZ,
    FILTER_5HZ,
} MPU_6050_filters_e;

extern HAL_StatusTypeDef MPU_6050_receive_accelerometer(int16_t* accelerometer);

extern HAL_StatusTypeDef MPU_6050_receive_gyroscope(int16_t* gyroscope);

extern HAL_StatusTypeDef MPU_6050_receive_temperature(int16_t* temperature);

extern HAL_StatusTypeDef MPU_6050_request_temperature();

extern HAL_StatusTypeDef MPU_6050_request_accelerometer();

extern HAL_StatusTypeDef MPU_6050_request_gyroscope();

extern HAL_StatusTypeDef MPU_6050_set_filter(MPU_6050_filters_e filter);

extern HAL_StatusTypeDef MPU_6050_disable_sleep();

#endif /* INC_MPU_6050_H_ */
