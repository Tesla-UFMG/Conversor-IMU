/*
 * CAN.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "CAN_Handler.h"

HAL_StatusTypeDef CAN_initialize();
void CAN_transmit(uint32_t id, uint16_t* data);
void CAN_receive_callback(CAN_HandleTypeDef* hcan);

#endif /* INC_CAN_H_ */
