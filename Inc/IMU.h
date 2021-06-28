#ifndef __IMU_H__
#define __IMU_H__


#include "main.h"
#include "stm32f1xx_hal.h"
#include "strings.h"
#include "i2c.h"
#include "gpio.h"

uint8_t _accel_ok;
int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ, temp;
void SetupACEL(void); //Funcao de inicializacao do MPU
void recordAccelRegisters(void); //Funcao de leitura do Acelerometro
void recordGyroRegisters(void); //funcao de leitura do Giroscopio
//void temperatura(void); //desnecessario
uint8_t Evita_travamento(uint32_t timer);
void Pisca_SOS();//pisca o led de debug em caso de timeout do IMU
uint8_t Checa_leitura();//verifica se ouve timout do imu
#define addressACEL 0x68


#endif
