/*
 * MPU_6050.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "MPU_6050.h"

static HAL_StatusTypeDef MPU_6050_read_measurement(int16_t* pData, uint8_t size);

static HAL_StatusTypeDef MPU_6050_read_data(uint8_t* buffer, uint16_t buffer_size);

static HAL_StatusTypeDef MPU_6050_send_command(uint8_t buffer);

static HAL_StatusTypeDef MPU_6050_write_memory(uint16_t memory_address, uint8_t* p_data);

#define MPU_6050_I2C_ADDRESS    (0x68 << 1)
#define DEFAULT_TIMEOUT         10
#define SEND_COMMAND_SIZE       1
#define ACCELEROMETER_DATA_SIZE 6
#define GYROSCOPE_DATA_SIZE     6
#define TEMPERATURE_DATA_SIZE   2

#define MPU_6050_CONFIG_REGISTER              0x1A
#define MPU_6050_ACCELEROMETER_SCALE_REGISTER 0x1C
#define MPU_6050_GYROSCOPE_SCALE_REGISTER     0x1B
#define MPU_6050_POWER_REGISTER               0x6B

#define MPU_6050_ACCELEROMETER_REGISTER 0x3B
#define MPU_6050_GYROSCOPE_REGISTER     0x43
#define MPU_6050_TEMPERATURE_REGISTER   0x41

/**
 * @brief Values provided at MPU6050 documentation
 *
 */
#define TEMPERATURE_GAIN   (1 / 34.0)
#define TEMPERATURE_OFFSET (365.3)

extern I2C_HandleTypeDef hi2c1;

static float accelerometer_gain = 1;
static float gyroscope_gain     = 1;

HAL_StatusTypeDef MPU_6050_receive_accelerometer(accelerometer_t* p_accelerometer) {
    int16_t accelerometer_raw[3];

    const HAL_StatusTypeDef status =
        MPU_6050_read_measurement(accelerometer_raw, ACCELEROMETER_DATA_SIZE);
    if (status != HAL_OK) {
        return status;
    }

    p_accelerometer->x = (int16_t)accelerometer_raw[0] / accelerometer_gain;
    p_accelerometer->y = (int16_t)accelerometer_raw[1] / accelerometer_gain;
    p_accelerometer->z = (int16_t)accelerometer_raw[2] / accelerometer_gain;
    return HAL_OK;
}

HAL_StatusTypeDef MPU_6050_receive_gyroscope(gyroscope_t* p_gyroscope) {
    int16_t gyroscope_raw[3];

    const HAL_StatusTypeDef status =
        MPU_6050_read_measurement(gyroscope_raw, GYROSCOPE_DATA_SIZE);
    if (status != HAL_OK) {
        return status;
    }
    p_gyroscope->x = (int16_t)gyroscope_raw[0] / gyroscope_gain;
    p_gyroscope->y = (int16_t)gyroscope_raw[1] / gyroscope_gain;
    p_gyroscope->z = (int16_t)gyroscope_raw[2] / gyroscope_gain;

    return HAL_OK;
}

HAL_StatusTypeDef MPU_6050_receive_temperature(int16_t* p_temperature) {
    const HAL_StatusTypeDef status =
        MPU_6050_read_measurement(p_temperature, TEMPERATURE_DATA_SIZE);
    *p_temperature = (int16_t)((*p_temperature * TEMPERATURE_GAIN) + TEMPERATURE_OFFSET);
    return status;
}

HAL_StatusTypeDef MPU_6050_request_temperature() {
    return MPU_6050_send_command(MPU_6050_TEMPERATURE_REGISTER);
}

HAL_StatusTypeDef MPU_6050_request_accelerometer() {
    return MPU_6050_send_command(MPU_6050_ACCELEROMETER_REGISTER);
}

HAL_StatusTypeDef MPU_6050_request_gyroscope() {
    return MPU_6050_send_command(MPU_6050_GYROSCOPE_REGISTER);
}

HAL_StatusTypeDef MPU_6050_set_accelerometer_scale(MPU_6050_accelerometer_scale_e scale) {
    switch (scale) {
        // Values obtained from MPU-6050 register map:
        case ACCELEROMETER_SCALE_16G: {
            accelerometer_gain = 2.048f;
            break;
        }
        case ACCELEROMETER_SCALE_8G: {
            accelerometer_gain = 4.096f;
            break;
        }
        case ACCELEROMETER_SCALE_4G: {
            accelerometer_gain = 8.192f;
            break;
        }
        case ACCELEROMETER_SCALE_2G:
        default: {
            accelerometer_gain = 16.384f;
            break;
        }
    }
    scale <<= 3;
    return MPU_6050_write_memory(MPU_6050_ACCELEROMETER_SCALE_REGISTER, &scale);
}

HAL_StatusTypeDef MPU_6050_set_gyroscope_scale(MPU_6050_gyroscope_scale_e scale) {
    switch (scale) {
        // Values obtained from MPU-6050 register map:
        case GYROSCOPE_SCALE_2000: {
            gyroscope_gain = 1.64;
            break;
        }
        case GYROSCOPE_SCALE_1000: {
            gyroscope_gain = 3.28;
            break;
        }
        case GYROSCOPE_SCALE_500: {
            gyroscope_gain = 6.55;
            break;
        }
        case GYROSCOPE_SCALE_250:
        default: {
            gyroscope_gain = 13.1;
            break;
        }
    }
    scale <<= 3;
    return MPU_6050_write_memory(MPU_6050_GYROSCOPE_SCALE_REGISTER, &scale);
}

HAL_StatusTypeDef MPU_6050_set_filter(MPU_6050_filters_e filter) {
    return MPU_6050_write_memory(MPU_6050_CONFIG_REGISTER, &filter);
}

HAL_StatusTypeDef MPU_6050_disable_sleep() {
    uint8_t data = 0b00000000;
    return MPU_6050_write_memory(MPU_6050_POWER_REGISTER, &data);
}

static HAL_StatusTypeDef MPU_6050_read_measurement(int16_t* p_data, uint8_t size) {
    uint8_t buffer[size];
    const HAL_StatusTypeDef status = MPU_6050_read_data(buffer, size);
    if (status != HAL_OK) {
        return status;
    }

    for (uint8_t i = 0; i < size / 2; ++i) {
        p_data[i] = (int16_t)(buffer[i * 2] << 8 | buffer[i * 2 + 1]);
    }

    return HAL_OK;
}

static HAL_StatusTypeDef MPU_6050_read_data(uint8_t* buffer, uint16_t buffer_size) {
    return (HAL_I2C_Master_Receive(&hi2c1, MPU_6050_I2C_ADDRESS, buffer, buffer_size,
                                   DEFAULT_TIMEOUT));
}

static HAL_StatusTypeDef MPU_6050_send_command(uint8_t buffer) {
    return (HAL_I2C_Master_Transmit(&hi2c1, MPU_6050_I2C_ADDRESS, &buffer,
                                    SEND_COMMAND_SIZE, DEFAULT_TIMEOUT));
}

static HAL_StatusTypeDef MPU_6050_write_memory(uint16_t memory_address, uint8_t* p_data) {
    return (HAL_I2C_Mem_Write(&hi2c1, MPU_6050_I2C_ADDRESS, memory_address,
                              SEND_COMMAND_SIZE, p_data, SEND_COMMAND_SIZE,
                              DEFAULT_TIMEOUT));
}
