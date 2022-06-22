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
void SetupACEL(void) {
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
}

int16_t calculate_accelerometer_gain(int16_t accelerometer_data) {
    return (int16_t)(accelerometer_data * ACCELEROMETER_GAIN);
}

HAL_StatusTypeDef get_accelerometer_value(accelerometer_t accelerometer) {

    timer = HAL_GetTick();
    // Inicializa a comunicacao com o registrador do acelerometro
    while (MPU_6050_request_accelerometer() != HAL_OK && Evita_travamento(timer)) {
        ;
    }

    timer             = HAL_GetTick();
    uint8_t buffer[6] = {0};
    while (MPU_6050_receive_accelerometer(buffer) != HAL_OK && Evita_travamento(timer)) {
        ;
    }

    accelerometer.x = calculate_accelerometer_gain((int16_t)(buffer[0] << 8 | buffer[1]))
                      + ACCELEROMETER_X_OFFSET;
    accelerometer.y = calculate_accelerometer_gain((int16_t)(buffer[2] << 8 | buffer[3]))
                      + ACCELEROMETER_Y_OFFSET;
    accelerometer.z = calculate_accelerometer_gain((int16_t)(buffer[4] << 8 | buffer[5]))
                      + ACCELEROMETER_Z_OFFSET;

    return HAL_OK;
}

int16_t calculate_gyroscope_gain(int16_t gyroscope_data) {
    return (int16_t)(gyroscope_data * GYROSCOPE_GAIN);
}

HAL_StatusTypeDef get_gyroscope_value(gyroscope_t gyroscope) {

    timer = HAL_GetTick();
    while (MPU_6050_request_gyroscope() != HAL_OK && Evita_travamento(timer)) {
        ;
    }

    timer             = HAL_GetTick();
    uint8_t buffer[6] = {0};
    while (MPU_6050_receive_gyroscope(buffer) != HAL_OK && Evita_travamento(timer)) {
        ;
    }

    gyroscope.x = calculate_gyroscope_gain((int16_t)(buffer[0] << 8 | buffer[1]))
                  + GYROSCOPE_X_OFFSET;
    gyroscope.y = calculate_gyroscope_gain((int16_t)(buffer[2] << 8 | buffer[3]))
                  + GYROSCOPE_Y_OFFSET;
    gyroscope.z = calculate_gyroscope_gain((int16_t)(buffer[4] << 8 | buffer[5]))
                  + GYROSCOPE_Z_OFFSET;
    return HAL_OK;
}

/*void temperatura(void) {
        buffer[0] = 0x41;
        timer = HAL_GetTick();
        while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) addressACEL << 1, buffer,
                        1, 100) != HAL_OK && Evita_travamento(timer));

        timer = HAL_GetTick();
        while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t) addressACEL << 1, buffer,
                        2, 100) != HAL_OK && Evita_travamento(timer));

        temp = (int16_t) (buffer[0] << 8 | buffer[1]);
        temp = temp / 340 + (int16_t) 36.53;
}*/

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
