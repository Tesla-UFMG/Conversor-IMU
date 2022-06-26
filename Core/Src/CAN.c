/*
 * CAN.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "CAN.h"

extern CAN_HandleTypeDef hcan;

static CAN_TxHeaderTypeDef TxHeader;

static uint8_t rx_data[8];
static CAN_RxHeaderTypeDef RxHeader;

uint16_t concatenate_two_uint8_to_uint16(const uint8_t* data) {
    return (data[1] << 8 | data[0]);
}

// Initialize the inverter CAN. Called in initializer.c
void CAN_initialize() {
    void CAN_receive_callback(CAN_HandleTypeDef*);
    CAN_handler_initialize(&hcan, CAN_receive_callback, &TxHeader);
}

// Function used to send a message via can
void CAN_transmit(uint32_t id, uint16_t* data) {
    CAN_handler_transmit(&hcan, &TxHeader, id, data);
}

// Callback function called when any inverter message is received via CAN
void CAN_receive_callback(CAN_HandleTypeDef* hcan) {
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rx_data) != HAL_OK) {
        /* Reception Error */
        // Error_Handler();
    }

    uint32_t id = RxHeader.StdId;
    for (int i = 0; i < 4; ++i) {
        uint16_t data = concatenate_two_uint8_to_uint16(rx_data + i * 2);
    }

    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        // Error_Handler();
    }
}
