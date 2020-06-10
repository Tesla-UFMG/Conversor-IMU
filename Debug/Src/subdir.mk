################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/IMU.c \
../Src/can.c \
../Src/can_log.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/IMU.o \
./Src/can.o \
./Src/can_log.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/IMU.d \
./Src/can.d \
./Src/can_log.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Eduardo/Documents/Tesla_UFMG/Controle/Conversor IMU 2.0/Inc" -I"C:/Users/Eduardo/Documents/Tesla_UFMG/Controle/Conversor IMU 2.0/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Eduardo/Documents/Tesla_UFMG/Controle/Conversor IMU 2.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Eduardo/Documents/Tesla_UFMG/Controle/Conversor IMU 2.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Eduardo/Documents/Tesla_UFMG/Controle/Conversor IMU 2.0/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


