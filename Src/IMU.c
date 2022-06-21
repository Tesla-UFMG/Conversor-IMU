#include <IMU.h>
#include <i2c.h>

uint8_t buffer[6] = {0};

int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ, temp;
uint8_t _accel_ok;

uint32_t timer = 0, timer1;
// uint16_t MemAdd = 0x6B;
void SetupACEL(void) {
    _accel_ok       = 1;
    uint16_t MemAdd = 0x6B;
    buffer[0]       = 0;
    timer           = HAL_GetTick();
    // Desabilita o sleep mode do MPU
    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;

    MemAdd    = (uint16_t)0x1B;
    buffer[0] = 0;
    timer     = HAL_GetTick();
    // Configura o giroscopio com a sensibilidade de 250
    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;

    MemAdd    = (uint16_t)0x1C;
    buffer[0] = 0;
    timer     = HAL_GetTick();
    // Configura o acelerometro com a sensibilidade de 4G

    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;
    MemAdd    = (uint16_t)0x1A;
    buffer[0] = 6;
    timer     = HAL_GetTick();
    // Configura o low pass filter interno do MPU como 6Hz
    while (
        HAL_I2C_Mem_Write(&hi2c1, (uint16_t)addressACEL << 1, MemAdd, 1, buffer, 1, 100)
            != HAL_OK
        && Evita_travamento(timer))
        ;
    accelX    = 0;
    accelY    = 0;
    accelZ    = 0;
    gyroX     = 0;
    gyroY     = 0;
    gyroZ     = 0;
    _accel_ok = 0;
}

void recordAccelRegisters() {

    buffer[0] = 0x3B;
    timer     = HAL_GetTick();
    // Inicializa a comunicacao com o registrador do acelerometro
    while (HAL_I2C_Master_Transmit(&hi2c1, (addressACEL << 1), buffer, 1, 100) != HAL_OK
           && Evita_travamento(timer))
        ;
    timer = HAL_GetTick();
    // Recebe os dados do acelerometro
    while (HAL_I2C_Master_Receive(&hi2c1, (addressACEL << 1), buffer, 6, 100) != HAL_OK
           && Evita_travamento(timer))
        ;
    // buffer[0] == HSB **** buffer[1] == LSB de uma variavel de 16 bits
    // buffer[2] == HSB **** buffer[3] == LSB de uma variavel de 16 bits
    // buffer[4] == HSB **** buffer[5] == LSB de uma variavel de 16 bits
    accelX = (int16_t)(buffer[0] << 8 | buffer[1]);
    accelY = (int16_t)(buffer[2] << 8 | buffer[3]);
    accelZ = (int16_t)(buffer[4] << 8 | buffer[5]);
    accelX = (accelX / 16384.0) * 1000;
    accelY = (accelY / 16384.0) * 1000;
    accelZ = (accelZ / 16384.0) * 1000;
    accelY = accelY - 20;
    accelZ = accelZ - 147;
}

void recordGyroRegisters(void) {

    buffer[0] = 0x43;
    timer     = HAL_GetTick();
    while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)addressACEL << 1, buffer, 1, 100)
               != HAL_OK
           && Evita_travamento(timer))
        ;

    timer = HAL_GetTick();
    while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)addressACEL << 1, buffer, 6, 100)
               != HAL_OK
           && Evita_travamento(timer))
        ;

    gyroX = (int16_t)(buffer[0] << 8 | buffer[1]);
    gyroY = (int16_t)(buffer[2] << 8 | buffer[3]);
    gyroZ = (int16_t)(buffer[4] << 8 | buffer[5]);
    gyroX = (gyroX / 131);
    gyroY = (gyroY / 131);
    gyroZ = (gyroZ / 131);
    gyroX = gyroX + 3;
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

uint8_t Evita_travamento(uint32_t timer) { // verifica se houve timeout no i2c
    uint32_t timer2 = HAL_GetTick();
    if ((timer2 - timer) < 50)
        return 1;
    else {
        _accel_ok = 1;
        return 0;
    }
}

// Caso haja falha na conexão do IMU com a placa, o LED_DEBUG piscar S.O.S em código morse
void Pisca_SOS() {

    HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, GPIO_PIN_RESET);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(167);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
    HAL_Delay(50);
}

// se houver timeout no i2c essa função sinaliza
uint8_t Checa_leitura() {
    return _accel_ok;
}
