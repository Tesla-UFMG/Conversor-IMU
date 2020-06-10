#include "main.h"
#include "stm32f1xx_hal.h"
#include "strings.h"
#include "i2c.h"
#include "gpio.h"

void SetupACEL(void); //Funcao de inicializacao do MPU
void recordAccelRegisters(void); //Funcao de leitura do Acelerometro
void recordGyroRegisters(void); //funcao de leitura do Giroscopio
//void temperatura(void); //desnecessario
uint8_t Evita_travamento(uint32_t timer);

#define addressACEL 0x68
