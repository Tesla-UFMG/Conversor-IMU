#include <IMU.h>
#include <string.h>
I2C_HandleTypeDef hi2c1; //handle do i2c;
uint8_t buffer[6] = { 0 };
int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ, temp;
uint8_t _accel_ok = 1;
uint32_t timer = 0, timer1;

void SetupACEL(void) {
	uint16_t MemAdd = 0x6B;
	buffer[0] = 0;
	timer = HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1, (uint16_t) addressACEL << 1, MemAdd, 1,
			buffer, 1, 100) != HAL_OK && Evita_travamento(timer)); //Desabilita o sleep mode do MPU

	MemAdd = (uint16_t) 0x1B;
	buffer[0] = 0;
	timer = HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1, (uint16_t) addressACEL << 1, MemAdd, 1,
			buffer, 1, 100) != HAL_OK && Evita_travamento(timer)); //Configura o giroscopio com a sensibilidade de 250

	MemAdd = (uint16_t) 0x1C;
	buffer[0] = 0;
	timer = HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1, (uint16_t) addressACEL << 1, MemAdd, 1,
			buffer, 1, 100) != HAL_OK && Evita_travamento(timer)); //Configura o acelerometro com a sensibilidade de 4G

	MemAdd = (uint16_t) 0x1A;
	buffer[0] = 6;
	timer = HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1, (uint16_t) addressACEL << 1, MemAdd, 1,
			buffer, 1, 100) != HAL_OK && Evita_travamento(timer)); //Configura o low pass filter interno do MPU como 6Hz
}

void recordAccelRegisters() {
	buffer[0] = 0x3B;
	timer = HAL_GetTick();
	while (HAL_I2C_Master_Transmit(&hi2c1, (addressACEL << 1), buffer, 1, 100)
			!= HAL_OK && Evita_travamento(timer)); //Inicializa a comunicacao com o registrador do acelerometro

	timer = HAL_GetTick();
	while (HAL_I2C_Master_Receive(&hi2c1, (addressACEL << 1), buffer, 6, 100)
			!= HAL_OK && Evita_travamento(timer)); //Recebe os dados do acelerometro

	accelX = (int16_t) (buffer[0] << 8 | buffer[1]); //buffer[0] == HSB **** buffer[1] == LSB de uma variavel de 16 bits
	accelY = (int16_t) (buffer[2] << 8 | buffer[3]); //buffer[2] == HSB **** buffer[3] == LSB de uma variavel de 16 bits
	accelZ = (int16_t) (buffer[4] << 8 | buffer[5]); //buffer[4] == HSB **** buffer[5] == LSB de uma variavel de 16 bits
	accelX = (accelX / 16384.0) * 1000 - 37;
	accelY = (accelY / 16384.0) * 1000 - 21;
	accelZ = (accelZ / 16384.0) * 1000 + 117;
}

void recordGyroRegisters(void) {
	buffer[0] = 0x43;
	timer = HAL_GetTick();
	while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) addressACEL << 1, buffer,
			1, 100) != HAL_OK && Evita_travamento(timer));

	timer = HAL_GetTick();
	while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t) addressACEL << 1, buffer,
			6, 100) != HAL_OK && Evita_travamento(timer));

	gyroX = (int16_t) (buffer[0] << 8 | buffer[1]);
	gyroY = (int16_t) (buffer[2] << 8 | buffer[3]);
	gyroZ = (int16_t) (buffer[4] << 8 | buffer[5]);
	gyroX = (gyroX / 131);
	gyroY = (gyroY / 131);
	gyroZ = (gyroZ / 131);
}

/*void temperatura(void) {
	buffer[0] = 0x41;
	timer = HAL_GetTick();
	while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) addressACEL << 1, buffer,
			1, 100) != HAL_OK && Evita_travamento(timer));

	timer = HAL_GetTick();
	while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t) addressACEL << 1, buffer,
			2, 100) != HAL_OK && Evita_travamento(timer));

	temp = (int16_t) (buffer[0] << 8 | buffer[1]);
	temp = temp / 340 + (int16_t) 36.53;
}*/

uint8_t Evita_travamento(uint32_t timer) {
	uint32_t timer2 = HAL_GetTick();
	if ((timer2 - timer) < 50) return 1;
	else {
		_accel_ok = 0;
		return 0;
	}

}
