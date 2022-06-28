/*
 * application.h
 *
 *  Created on: Jun 21, 2022
 *      Author: iFeli
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

void application_initializer();
void application_run();

typedef enum { INIT_CAN = 0, INIT_IMU, RUNNING, ERROR_IMU } state_e;

typedef enum { WORD_0 = 0, WORD_1, WORD_2, WORD_3 } message_positions_e;
typedef enum { IMU_ERROR = 0, IMU_OK } imu_status_e;

#endif /* INC_APPLICATION_H_ */
