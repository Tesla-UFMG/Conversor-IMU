/*
 * Application.c
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#include "application.h"

#include "CAN.h"
#include "IMU.h"
#include "main.h"
#include "timer_handler.h"

static void application_state_machine(void);
static void led_state_machine(void);

static state_e application_state = STARTUP_DELAY;

static accelerometer_t accelerometer = {.x = 0, .y = 0, .z = 0};
static gyroscope_t gyroscope         = {.x = 0, .y = 0, .z = 0};
static int16_t temperature           = 0;

static uint32_t startup_timer       = 0;
static uint32_t accelerometer_timer = 0;
static uint32_t gyroscope_timer     = 0;
static uint32_t temperature_timer   = 0;
static uint32_t led_timer           = 0;

void application_run() {
    application_state_machine();
    led_state_machine();
}

static void application_state_machine(void) {
    switch (application_state) {
        case STARTUP_DELAY: {
            timer_restart(&startup_timer);
            application_state = INIT_CAN;
            break;
        }
        case INIT_CAN: {
            if (timer_wait_ms(startup_timer, 500)) {
                if (CAN_initialize() == HAL_OK) {
                    application_state = INIT_IMU;
                }
            }
            break;
        }
        case INIT_IMU: {
            if (IMU_initialize() == HAL_OK) {
                application_state = RUNNING;
            }
            break;
        }
        case RUNNING: {
            if (timer_wait_ms(temperature_timer, 1000)) {
                get_temperature_value(&temperature);
                timer_restart(&temperature_timer);
            }

            if (timer_wait_ms(accelerometer_timer, 50)) {
                if (get_accelerometer_value(&accelerometer) != HAL_OK) {
                    application_state = ERROR_IMU;
                    return;
                }

                uint16_t accelerometer_transmit[4];
                accelerometer_transmit[0] = accelerometer.x;
                accelerometer_transmit[1] = accelerometer.y;
                accelerometer_transmit[2] = accelerometer.z;
                accelerometer_transmit[3] = temperature;

                if (CAN_transmit(291, accelerometer_transmit) != HAL_OK) {
                    application_state = ERROR_IMU;
                    return;
                }
                timer_restart(&accelerometer_timer);
            }

            if (timer_wait_ms(gyroscope_timer, 20)) {

                if (get_gyroscope_value(&gyroscope) != HAL_OK) {
                    application_state = ERROR_IMU;
                    return;
                }

                uint16_t gyroscope_transmit[4];
                gyroscope_transmit[WORD_0] = gyroscope.x;
                gyroscope_transmit[WORD_1] = gyroscope.y;
                gyroscope_transmit[WORD_2] = gyroscope.z;
                gyroscope_transmit[WORD_3] = IMU_OK;

                if (CAN_transmit(292, gyroscope_transmit) != HAL_OK) {
                    application_state = ERROR_IMU;
                    return;
                }
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

            CAN_transmit(292, error_transmit);

            IMU_deinitialize();
            CAN_deinitialize();

            application_state = INIT_CAN;

            break;
        }
        default: break;
    }
}

static void led_state_machine(void) {
    switch (application_state) {
        case RUNNING:
            if (timer_wait_ms(led_timer, 500)) {
                HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
                timer_restart(&led_timer);
            }
            break;

        default:
            if (timer_wait_ms(led_timer, 50)) {
                HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
                timer_restart(&led_timer);
            }
            break;
    }
}
