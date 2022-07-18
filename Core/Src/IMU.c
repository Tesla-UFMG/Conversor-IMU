/*
 * IMU.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "IMU.h"

#define ACCELEROMETER_X_OFFSET (0)
#define ACCELEROMETER_Y_OFFSET (0)
#define ACCELEROMETER_Z_OFFSET (0)

#define GYROSCOPE_X_OFFSET (0)
#define GYROSCOPE_Y_OFFSET (0)
#define GYROSCOPE_Z_OFFSET (0)

HAL_StatusTypeDef IMU_initialize(void) {
    HAL_StatusTypeDef status;

    status = MPU_6050_disable_sleep();
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_set_filter(FILTER_21HZ);
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_set_accelerometer_scale(ACCELEROMETER_SCALE_4G);
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_set_gyroscope_scale(GYROSCOPE_SCALE_500);
    if (status != HAL_OK) {
        return status;
    }

    return HAL_OK;
}

HAL_StatusTypeDef get_accelerometer_value(accelerometer_t* accelerometer) {
    HAL_StatusTypeDef status;
    status = MPU_6050_request_accelerometer();
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_receive_accelerometer(accelerometer);
    if (status != HAL_OK) {
        return status;
    }

    accelerometer->x += ACCELEROMETER_X_OFFSET;
    accelerometer->y += ACCELEROMETER_Y_OFFSET;
    accelerometer->z += ACCELEROMETER_Z_OFFSET;

    return HAL_OK;
}

HAL_StatusTypeDef get_gyroscope_value(gyroscope_t* gyroscope) {
    HAL_StatusTypeDef status;
    status = MPU_6050_request_gyroscope();
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_receive_gyroscope(gyroscope);
    if (status != HAL_OK) {
        return status;
    }

    gyroscope->x += GYROSCOPE_X_OFFSET;
    gyroscope->y += GYROSCOPE_Y_OFFSET;
    gyroscope->z += GYROSCOPE_Z_OFFSET;

    return HAL_OK;
}

HAL_StatusTypeDef get_temperature_value(int16_t* temperature) {
    HAL_StatusTypeDef status;
    status = MPU_6050_request_temperature();
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_receive_temperature(temperature);
    if (status != HAL_OK) {
        return status;
    }

    return HAL_OK;
}
