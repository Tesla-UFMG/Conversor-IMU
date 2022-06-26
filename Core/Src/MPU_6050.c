/*
 * MPU_6050.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "stm32f1xx_hal.h"

#include "MPU_6050.h"

#define MPU_6050_I2C_ADDRESS (0x68 << 1)
#define DEFAULT_TIMEOUT      (100)
#define SEND_COMMAND_SIZE    (1)
#define RECEIVE_DATA_SIZE    (6)

#define MPU_6050_READ_GYROSCOPE_REGISTER     0x43
#define MPU_6050_READ_ACCELEROMETER_REGISTER 0x3B

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef MPU_6050_receive_accelerometer(uint8_t* accelerometer) {
    return MPU_6050_read_data(accelerometer);
}

HAL_StatusTypeDef MPU_6050_receive_gyroscope(uint8_t* gyroscope) {
    return MPU_6050_read_data(gyroscope);
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

HAL_StatusTypeDef MPU_6050_read_data(uint8_t* buffer) {
    return (HAL_I2C_Master_Receive(&hi2c1, MPU_6050_I2C_ADDRESS, buffer,
                                    RECEIVE_DATA_SIZE, DEFAULT_TIMEOUT));
}
