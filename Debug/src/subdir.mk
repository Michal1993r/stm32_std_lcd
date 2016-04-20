################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LCD_STM32F4.c \
../src/fonts.c \
../src/functions.c \
../src/main.c \
../src/pwm.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/ts.c 

OBJS += \
./src/LCD_STM32F4.o \
./src/fonts.o \
./src/functions.o \
./src/main.o \
./src/pwm.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/ts.o 

C_DEPS += \
./src/LCD_STM32F4.d \
./src/fonts.d \
./src/functions.d \
./src/main.d \
./src/pwm.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/ts.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F40XX -DSTM32F40_41xxx -I"C:/Users/MiCkl/Dysk Google/Programowanie/stm32_std_lcd/inc" -I"C:/Users/MiCkl/Dysk Google/Programowanie/stm32_std_lcd/CMSIS/core" -I"C:/Users/MiCkl/Dysk Google/Programowanie/stm32_std_lcd/CMSIS/device" -I"C:/Users/MiCkl/Dysk Google/Programowanie/stm32_std_lcd/StdPeriph_Driver/inc" -I"C:/Users/MiCkl/Dysk Google/Programowanie/stm32_std_lcd/Utilities" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


