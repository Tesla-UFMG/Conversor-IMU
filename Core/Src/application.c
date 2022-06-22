/*
 * Application.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "application.h"

#include "CAN.h"
#include "IMU.h"

static uint16_t accelerometer_transmit[4] = {0, 0, 0, 0};
static uint16_t gyroscope_transmit[4]     = {0, 0, 0, 0};

void application_initializer() {
    HAL_Delay(500);

    SetupACEL();
}
typedef enum { X = 0, Y, Z, STATUS } message_positions_e;
typedef enum { IMU_ERROR = 0, IMU_OK } imu_status_e;

void application_run() {
    accelerometer_t accelerometer = {0, 0, 0};
    gyroscope_t gyroscope         = {0, 0, 0};

    if (get_accelerometer_value(accelerometer) == HAL_OK) {
        accelerometer_transmit[X]      = accelerometer.x;
        accelerometer_transmit[Y]      = accelerometer.y;
        accelerometer_transmit[Z]      = accelerometer.z;
        accelerometer_transmit[STATUS] = IMU_ERROR;
    } else {
        accelerometer_transmit[STATUS] = IMU_OK;
    }
    CAN_transmit(192, accelerometer_transmit);

    if (get_gyroscope_value(gyroscope) == HAL_OK) {
        gyroscope_transmit[X]          = gyroscope.x;
        gyroscope_transmit[Y]          = gyroscope.y;
        gyroscope_transmit[Z]          = gyroscope.z;
        accelerometer_transmit[STATUS] = IMU_ERROR;
    } else {
        gyroscope_transmit[STATUS] = IMU_OK;
    }
    CAN_transmit(191, gyroscope_transmit);

    HAL_Delay(50);
    
    if (accelerometer_transmit[STATUS] == IMU_ERROR
        || gyroscope_transmit[STATUS] == IMU_ERROR) {
        SetupACEL();
    }
}