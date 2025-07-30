################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/keyboard.c \
../Device/led.c \
../Device/motor.c 

OBJS += \
./Device/ES8388.o \
./Device/keyboard.o \
./Device/led.o \
./Device/motor.o 

C_DEPS += \
./Device/ES8388.d \
./Device/keyboard.d \
./Device/led.d \
./Device/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\18-三相电机\WCH-CH32V307VCT6-MOTOR\Debug" -I"D:\WorkingProject\Module-Drive\18-三相电机\WCH-CH32V307VCT6-MOTOR\Driver" -I"D:\WorkingProject\Module-Drive\18-三相电机\WCH-CH32V307VCT6-MOTOR\Device" -I"D:\WorkingProject\Module-Drive\18-三相电机\WCH-CH32V307VCT6-MOTOR\Core" -I"D:\WorkingProject\Module-Drive\18-三相电机\WCH-CH32V307VCT6-MOTOR\User" -I"D:\WorkingProject\Module-Drive\18-三相电机\WCH-CH32V307VCT6-MOTOR\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

