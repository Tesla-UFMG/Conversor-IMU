/*
 * IMU.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "IMU.h"

#include "MPU_6050.h"

#define ACCELEROMETER_GAIN     (0.061035)
#define ACCELEROMETER_X_OFFSET (0)
#define ACCELEROMETER_Y_OFFSET (-20)
#define ACCELEROMETER_Z_OFFSET (-147)

#define GYROSCOPE_GAIN     (0.00763)
#define GYROSCOPE_X_OFFSET (3)
#define GYROSCOPE_Y_OFFSET (0)
#define GYROSCOPE_Z_OFFSET (0)

uint8_t _accel_ok;

uint32_t timer = 0, timer1;
// uint16_t MemAdd = 0x6B;
HAL_StatusTypeDef SetupACEL(void) {
    _accel_ok         = 1;
    uint16_t MemAdd   = 0x6B;
    uint8_t buffer[1] = {0};
    timer             = HAL_GetTick();
    // Desabilita o sleep mode do MPU
    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;

    MemAdd    = (uint16_t)0x1B;
    buffer[0] = 0;
    timer     = HAL_GetTick();
    // Configura o giroscopio com a sensibilidade de 250
    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;

    MemAdd    = (uint16_t)0x1C;
    buffer[0] = 0;
    timer     = HAL_GetTick();
    // Configura o acelerometro com a sensibilidade de 4G

    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;
    MemAdd    = (uint16_t)0x1A;
    buffer[0] = 6;
    timer     = HAL_GetTick();
    // Configura o low pass filter interno do MPU como 6Hz
    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;
    return HAL_OK;
}

int16_t calculate_accelerometer_gain(int16_t accelerometer_data) {
    return (int16_t)(accelerometer_data * ACCELEROMETER_GAIN);
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

int16_t calculate_gyroscope_gain(int16_t gyroscope_data) {
    return (int16_t)(gyroscope_data * GYROSCOPE_GAIN);
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

uint8_t Evita_travamento(uint32_t timer) { // verifica se houve timeout no i2c
    uint32_t timer2 = HAL_GetTick();
    if ((timer2 - timer) < 50)
        return 1;
    else {
        _accel_ok = 1;
        return 0;
    }
}

// Caso haja falha na conexão do IMU com a placa, o LED_DEBUG piscar S.O.S em código morse
void Pisca_SOS() {

    HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, GPIO_PIN_RESET);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
}

// se houver timeout no i2c essa função sinaliza
uint8_t Checa_leitura() {
    return _accel_ok;
}
