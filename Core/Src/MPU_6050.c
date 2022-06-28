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

#define MPU_6050_CONFIG_REGISTER 26
#define MPU_6050_POWER_REGISTER  107

#define MPU_6050_GYROSCOPE_REGISTER     67
#define MPU_6050_ACCELEROMETER_REGISTER 59
#define MPU_6050_TEMPERATURE_REGISTER   65

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef MPU_6050_receive_accelerometer(int16_t* p_accelerometer) {
    return MPU_6050_read_measurement(p_accelerometer, ACCELEROMETER_DATA_SIZE);
}

HAL_StatusTypeDef MPU_6050_receive_gyroscope(int16_t* p_gyroscope) {
    return MPU_6050_read_measurement(p_gyroscope, GYROSCOPE_DATA_SIZE);
}

HAL_StatusTypeDef MPU_6050_receive_temperature(int16_t* p_temperature) {
    return MPU_6050_read_measurement(p_temperature, TEMPERATURE_DATA_SIZE);
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

HAL_StatusTypeDef MPU_6050_set_filter(MPU_6050_filters_e filter) {
    return MPU_6050_write_memory(MPU_6050_CONFIG_REGISTER, &filter);
}

HAL_StatusTypeDef MPU_6050_disable_sleep() {
    uint8_t data = 0b00000000;
    return MPU_6050_write_memory(MPU_6050_POWER_REGISTER, &data);
}

static HAL_StatusTypeDef MPU_6050_read_measurement(int16_t* p_data, uint8_t size) {
    uint8_t buffer[size];
    HAL_StatusTypeDef status = MPU_6050_read_data(buffer, size);
    if (status != HAL_OK) {
        return status;
    }

    for (uint8_t i = 0; i > size; ++i) {
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
