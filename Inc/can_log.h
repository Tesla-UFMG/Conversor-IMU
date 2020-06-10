#ifndef CAN_LOG_H_
#define CAN_LOG_H_

//Includes:
#include "main.h"
#include "stm32f1xx_hal.h"

//Declaring functions:
#define CAN_IDS_NUMBER 360
#define REAL_CLK_CAN_ID 10

typedef struct{
  uint16_t word_0;
  uint16_t word_1;
  uint16_t word_2;
  uint16_t word_3;
}CanIdData_t;


void transmit_dados();
void Clean_CAN_Struct(void);
void canMessageReceived(uint16_t id,uint8_t* data);
void writeSD(void);
void readSD(void);
void cabecalho(void);
void balde_caixa(void);
void UART_print(char* format, ...);
void Header(void);
#endif //END CAN_LOG_H



