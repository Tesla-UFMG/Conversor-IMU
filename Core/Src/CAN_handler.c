/*
 * CAN_handler.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "CAN_handler.h"

uint32_t TxMailbox;

// Function to initialize CAN
HAL_StatusTypeDef
CAN_handler_initialize(CAN_HandleTypeDef* hcan,
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

    CAN_FilterTypeDef canfilterconfig;

    canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
    canfilterconfig.FilterBank = 0; // which filter bank to use from the assigned ones
    canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    canfilterconfig.FilterIdHigh         = 0x0000;
    canfilterconfig.FilterIdLow          = 0x0000;
    canfilterconfig.FilterMaskIdHigh     = 0x0000;
    canfilterconfig.FilterMaskIdLow      = 0x0000;
    canfilterconfig.FilterMode           = CAN_FILTERMODE_IDMASK;
    canfilterconfig.FilterScale          = CAN_FILTERSCALE_32BIT;
    canfilterconfig.SlaveStartFilterBank =
        14; // how many filters to assign to the CAN1 (master can)

    HAL_CAN_ConfigFilter(hcan, &canfilterconfig);

    if (HAL_CAN_Start(hcan) != HAL_OK) {
        /* Start Error */
        // Error_Handler(); // NOLINT
    }

    TxHeader->ExtId              = 0x01;
    TxHeader->RTR                = CAN_RTR_DATA;
    TxHeader->IDE                = CAN_ID_STD;
    TxHeader->DLC                = 8;
    TxHeader->TransmitGlobalTime = DISABLE;
    return HAL_OK;
}

HAL_StatusTypeDef CAN_handler_transmit(CAN_HandleTypeDef* hcan,
                                       CAN_TxHeaderTypeDef* TxHeader, uint32_t id,
                                       uint16_t* data) {
    TxHeader->StdId = id;
    return HAL_CAN_AddTxMessage(hcan, TxHeader, (uint8_t*)data, &TxMailbox);
}
