/*
 * CAN_handler.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_CAN_HANDLER_H_
#define INC_CAN_HANDLER_H_

#include "stm32f1xx_hal.h"

HAL_StatusTypeDef
CAN_handler_initialize(CAN_HandleTypeDef* hcan,
                       void (*CAN_receive_callback)(CAN_HandleTypeDef* hcan),
                       CAN_TxHeaderTypeDef* TxHeader);

HAL_StatusTypeDef CAN_handler_deinitialize(CAN_HandleTypeDef* hcan);

HAL_StatusTypeDef CAN_handler_transmit(CAN_HandleTypeDef* hcan,
                                       CAN_TxHeaderTypeDef* TxHeader, uint32_t id,
                                       uint16_t* data);

#endif /* INC_CAN_HANDLER_H_ */
