//Includes:
#include <can.h>
#include <IMU.h>
#include "can_log.h"
#include <string.h>
#include <main.h>
#include <stdio.h>


extern int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ;

void transmit_dados() {
	uint8_t vetTx[8];
	vetTx[0] = accelX;
	vetTx[1] = accelX >> 8;
	vetTx[2] = accelY;
	vetTx[3] = accelY >> 8;
	vetTx[4] = accelZ;
	vetTx[5] = accelZ >> 8;
	vetTx[6] = 0;
	vetTx[7] = 0;
	CAN_Transmit(vetTx, 108);
	vetTx[0] = gyroX;
	vetTx[1] = gyroX >> 8;
	vetTx[2] = gyroY;
	vetTx[3] = gyroY >> 8;
	vetTx[4] = gyroZ;
	vetTx[5] = gyroZ >> 8;
	vetTx[6] = 0;
	vetTx[7] = 0;
	CAN_Transmit(vetTx, 109);
}
