/*
 * IMU.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "main.h"
#include "strings.h"

#define addressACEL 0x68

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

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef get_temperature_value(int16_t* temperature);
extern uint8_t _accel_ok;
extern int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ, temp;
void SetupACEL(void); // Funcao de inicializacao do MPU
HAL_StatusTypeDef get_accelerometer_value(accelerometer_t* accelerometer);
HAL_StatusTypeDef get_gyroscope_value(gyroscope_t* gyroscope);
// void temperatura(void); //desnecessario
uint8_t Evita_travamento(uint32_t timer);
void Pisca_SOS();        // pisca o led de debug em caso de timeout do IMU
uint8_t Checa_leitura(); // verifica se ouve timout do imu

#endif /* INC_IMU_H_ */
