################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/Joystick.c \
../Device/led.c 

OBJS += \
./Device/ES8388.o \
./Device/Joystick.o \
./Device/led.o 

C_DEPS += \
./Device/ES8388.d \
./Device/Joystick.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\07-摇杆\WCH-CH32V307VCT6-Joystick\Debug" -I"D:\WorkingProject\Module-Drive\07-摇杆\WCH-CH32V307VCT6-Joystick\Driver" -I"D:\WorkingProject\Module-Drive\07-摇杆\WCH-CH32V307VCT6-Joystick\Device" -I"D:\WorkingProject\Module-Drive\07-摇杆\WCH-CH32V307VCT6-Joystick\Core" -I"D:\WorkingProject\Module-Drive\07-摇杆\WCH-CH32V307VCT6-Joystick\User" -I"D:\WorkingProject\Module-Drive\07-摇杆\WCH-CH32V307VCT6-Joystick\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

