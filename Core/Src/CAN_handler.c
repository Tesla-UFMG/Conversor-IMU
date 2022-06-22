/*
 * CAN_handler.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "CAN_handler.h"

uint32_t TxMailbox;

// Function to initialize CAN
void CAN_handler_initialize(CAN_HandleTypeDef* hcan,
                            void (*CAN_receive_callback)(CAN_HandleTypeDef* hcan),
                            CAN_TxHeaderTypeDef* TxHeader) {

    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        // Error_Handler(); // NOLINT
    }

    // Function to register the custom CAN receive callback
    if (HAL_CAN_RegisterCallback(hcan, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID,
                                 CAN_receive_callback)
        != HAL_OK) {
        /* Callback Register Error */
        // Error_Handler(); // NOLINT
    }

    if (HAL_CAN_Start(hcan) != HAL_OK) {
        /* Start Error */
        // Error_Handler(); // NOLINT
    }

    TxHeader->ExtId              = 0x01;
    TxHeader->RTR                = CAN_RTR_DATA;
    TxHeader->IDE                = CAN_ID_STD;
    TxHeader->DLC                = 8;
    TxHeader->TransmitGlobalTime = DISABLE;
}

HAL_StatusTypeDef CAN_handler_transmit(CAN_HandleTypeDef* hcan,
                                       CAN_TxHeaderTypeDef* TxHeader, uint32_t id,
                                       uint16_t* data) {
    TxHeader->StdId = id;
    return HAL_CAN_AddTxMessage(hcan, TxHeader, (uint8_t*)data, &TxMailbox);
}
