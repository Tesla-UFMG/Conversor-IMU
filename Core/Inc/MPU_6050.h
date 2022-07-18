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

typedef enum {
    ACCELEROMETER_SCALE_2G = 0,
    ACCELEROMETER_SCALE_4G,
    ACCELEROMETER_SCALE_8G,
    ACCELEROMETER_SCALE_16G,
} MPU_6050_accelerometer_scale_e;

typedef enum {
    GYROSCOPE_SCALE_250 = 0,
    GYROSCOPE_SCALE_500,
    GYROSCOPE_SCALE_1000,
    GYROSCOPE_SCALE_2000,
} MPU_6050_gyroscope_scale_e;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} accelerometer_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} gyroscope_t;

extern HAL_StatusTypeDef MPU_6050_receive_accelerometer(accelerometer_t* accelerometer);

extern HAL_StatusTypeDef MPU_6050_receive_gyroscope(gyroscope_t* p_gyroscope);

extern HAL_StatusTypeDef MPU_6050_receive_temperature(int16_t* temperature);

extern HAL_StatusTypeDef MPU_6050_request_temperature();

extern HAL_StatusTypeDef MPU_6050_request_accelerometer();

extern HAL_StatusTypeDef MPU_6050_request_gyroscope();

HAL_StatusTypeDef MPU_6050_set_accelerometer_scale(MPU_6050_accelerometer_scale_e scale);

HAL_StatusTypeDef MPU_6050_set_gyroscope_scale(MPU_6050_gyroscope_scale_e scale);

extern HAL_StatusTypeDef MPU_6050_set_filter(MPU_6050_filters_e filter);

extern HAL_StatusTypeDef MPU_6050_disable_sleep();

#endif /* INC_MPU_6050_H_ */
