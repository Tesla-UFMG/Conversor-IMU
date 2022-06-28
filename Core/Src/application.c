/*
 * Application.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "application.h"

#include "CAN.h"
#include "IMU.h"
#include "timer_handler.h"

static state_e state;

static accelerometer_t accelerometer = {.x = 0, .y = 0, .z = 0};
static gyroscope_t gyroscope         = {.x = 0, .y = 0, .z = 0};
static int16_t temperature           = 0;

static uint32_t accelerometer_timer = 0;
static uint32_t gyroscope_timer     = 0;
static uint32_t temperature_timer   = 0;

void application_run() {
    application_state_machine();
}

void application_state_machine(void) {
    switch (state) {
        case INIT_CAN: {
            if (CAN_initialize() == HAL_OK) {
                state = INIT_IMU;
            }
            break;
        }
        case INIT_IMU: {
            if (SetupACEL() == HAL_OK) {
                state = RUNNING;
            }
            break;
        }
        case RUNNING: {
            if (timer_wait(temperature_timer, 1000)) {
                get_temperature_value(&temperature);
                timer_restart(&temperature_timer);
            }

            if (timer_wait(accelerometer_timer, 50)) {
                if (get_accelerometer_value(&accelerometer) != HAL_OK) {
                    state = ERROR_IMU;
                    return;
                }

                uint16_t accelerometer_transmit[4];
                accelerometer_transmit[0] = accelerometer.x;
                accelerometer_transmit[1] = accelerometer.y;
                accelerometer_transmit[2] = accelerometer.z;
                accelerometer_transmit[3] = temperature;

                CAN_transmit(130, accelerometer_transmit);
                timer_restart(&accelerometer_timer);
            }

            if (timer_wait(gyroscope_timer, 20)) {

                if (get_gyroscope_value(&gyroscope) != HAL_OK) {
                    state = ERROR_IMU;
                    return;
                }

                uint16_t gyroscope_transmit[4];
                gyroscope_transmit[WORD_0] = gyroscope.x;
                gyroscope_transmit[WORD_1] = gyroscope.y;
                gyroscope_transmit[WORD_2] = gyroscope.z;
                gyroscope_transmit[WORD_3] = IMU_OK;

                CAN_transmit(131, gyroscope_transmit);
                timer_restart(&gyroscope_timer);
            }

            break;
        }
        case ERROR_IMU: {
            uint16_t error_transmit[4];

            error_transmit[WORD_0] = 0;
            error_transmit[WORD_1] = 0;
            error_transmit[WORD_2] = 0;
            error_transmit[WORD_3] = IMU_ERROR;

            CAN_transmit(131, error_transmit);

            state = INIT_IMU;

            break;
        }
        default: break;
    }
}
