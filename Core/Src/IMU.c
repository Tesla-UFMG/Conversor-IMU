/*
 * IMU.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "IMU.h"

#include "MPU_6050.h"

static int16_t calculate_accelerometer_gain(int16_t accelerometer_data);

static int16_t calculate_gyroscope_gain(int16_t gyroscope_data);

#define ACCELEROMETER_GAIN     0.061035
#define ACCELEROMETER_X_OFFSET 0
#define ACCELEROMETER_Y_OFFSET (-20)
#define ACCELEROMETER_Z_OFFSET (-147)

#define GYROSCOPE_GAIN     0.00763
#define GYROSCOPE_X_OFFSET 3
#define GYROSCOPE_Y_OFFSET 0
#define GYROSCOPE_Z_OFFSET 0

/**
 * @brief Values provided at MPU6050 documentation
 *
 */
#define TEMPERATURE_GAIN   (1 / 34.0)
#define TEMPERATURE_OFFSET (365.3)

HAL_StatusTypeDef IMU_initialize(void) {
    HAL_StatusTypeDef status;

    status = MPU_6050_disable_sleep();
    if (status != HAL_OK) {
        return status;
    }

    status = MPU_6050_set_filter(FILTER_5HZ);
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

    int16_t buffer[3];
    status = MPU_6050_receive_accelerometer(buffer);
    if (status != HAL_OK) {
        return status;
    }

    accelerometer->x = calculate_accelerometer_gain(buffer[0]) + ACCELEROMETER_X_OFFSET;
    accelerometer->y = calculate_accelerometer_gain(buffer[1]) + ACCELEROMETER_Y_OFFSET;
    accelerometer->z = calculate_accelerometer_gain(buffer[2]) + ACCELEROMETER_Z_OFFSET;

    return HAL_OK;
}

HAL_StatusTypeDef get_gyroscope_value(gyroscope_t* gyroscope) {
    HAL_StatusTypeDef status;
    status = MPU_6050_request_gyroscope();
    if (status != HAL_OK) {
        return status;
    }

    int16_t buffer[3];
    status = MPU_6050_receive_gyroscope(buffer);
    if (status != HAL_OK) {
        return status;
    }

    gyroscope->x = calculate_gyroscope_gain(buffer[0]) + GYROSCOPE_X_OFFSET;
    gyroscope->y = calculate_gyroscope_gain(buffer[1]) + GYROSCOPE_Y_OFFSET;
    gyroscope->z = calculate_gyroscope_gain(buffer[2]) + GYROSCOPE_Z_OFFSET;
    return HAL_OK;
}

HAL_StatusTypeDef get_temperature_value(int16_t* temperature) {
    HAL_StatusTypeDef status;
    status = MPU_6050_request_temperature();
    if (status != HAL_OK) {
        return status;
    }

    int16_t buffer;
    status = MPU_6050_receive_temperature(&buffer);
    if (status != HAL_OK) {
        return status;
    }

    *temperature = (buffer / 34 + 365.3);

    return HAL_OK;
}

static int16_t calculate_accelerometer_gain(int16_t accelerometer_data) {
    return (int16_t)(accelerometer_data * ACCELEROMETER_GAIN);
}

static int16_t calculate_gyroscope_gain(int16_t gyroscope_data) {
    return (int16_t)(gyroscope_data * GYROSCOPE_GAIN);
}

static int16_t calculate_temperature_gain(int16_t temperature_data) {
    return (int16_t)((temperature_data * TEMPERATURE_GAIN) + TEMPERATURE_OFFSET);
}