/*
 * MPU_6050.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "MPU_6050.h"

#include "stm32f1xx_hal.h"

#define MPU_6050_I2C_ADDRESS    (0x68 << 1)
#define DEFAULT_TIMEOUT         (100)
#define SEND_COMMAND_SIZE       (1)
#define ACCELEROMETER_DATA_SIZE (6)
#define GYROSCOPE_DATA_SIZE     (6)
#define TEMPERATURE_DATA_SIZE   (2)

#define MPU_6050_READ_GYROSCOPE_REGISTER     0x43
#define MPU_6050_READ_ACCELEROMETER_REGISTER 0x3B
#define MPU_6050_READ_TEMPERATURE_REGISTER   0x41

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef MPU_6050_receive_accelerometer(int16_t* accelerometer) {
    uint8_t buffer[ACCELEROMETER_DATA_SIZE];
    HAL_StatusTypeDef status = MPU_6050_read_data(buffer, ACCELEROMETER_DATA_SIZE);

    accelerometer[0] = (buffer[0] << 8 | buffer[1]);
    accelerometer[1] = (buffer[2] << 8 | buffer[3]);
    accelerometer[2] = (buffer[4] << 8 | buffer[5]);

    return status;
}

HAL_StatusTypeDef MPU_6050_receive_gyroscope(int16_t* gyroscope) {
    uint8_t buffer[GYROSCOPE_DATA_SIZE];
    HAL_StatusTypeDef status = MPU_6050_read_data(buffer, GYROSCOPE_DATA_SIZE);

    gyroscope[0] = (int16_t)(buffer[0] << 8 | buffer[1]);
    gyroscope[1] = (int16_t)(buffer[2] << 8 | buffer[3]);
    gyroscope[2] = (int16_t)(buffer[4] << 8 | buffer[5]);

    return status;
}

HAL_StatusTypeDef MPU_6050_receive_temperature(int16_t* temperature) {
    uint8_t buffer[TEMPERATURE_DATA_SIZE];
    HAL_StatusTypeDef status = MPU_6050_read_data(buffer, TEMPERATURE_DATA_SIZE);

    *temperature = (int16_t)(buffer[0] << 8 | buffer[1]);

    return status;
}

HAL_StatusTypeDef MPU_6050_request_temperature() {
    return MPU_6050_send_command(MPU_6050_READ_TEMPERATURE_REGISTER);
}

HAL_StatusTypeDef MPU_6050_request_accelerometer() {
    return MPU_6050_send_command(MPU_6050_READ_ACCELEROMETER_REGISTER);
}

HAL_StatusTypeDef MPU_6050_request_gyroscope() {
    return MPU_6050_send_command(MPU_6050_READ_GYROSCOPE_REGISTER);
}

HAL_StatusTypeDef MPU_6050_send_command(uint8_t buffer) {
    return (HAL_I2C_Master_Transmit(&hi2c1, MPU_6050_I2C_ADDRESS, &buffer,
                                    SEND_COMMAND_SIZE, DEFAULT_TIMEOUT));
}

HAL_StatusTypeDef MPU_6050_read_data(uint8_t* buffer, uint16_t buffer_size) {
    return (HAL_I2C_Master_Receive(&hi2c1, MPU_6050_I2C_ADDRESS, buffer, buffer_size,
                                   DEFAULT_TIMEOUT));
}
