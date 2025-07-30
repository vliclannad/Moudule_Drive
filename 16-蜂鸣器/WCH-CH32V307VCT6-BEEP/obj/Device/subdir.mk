################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/beep.c \
../Device/led.c 

OBJS += \
./Device/ES8388.o \
./Device/beep.o \
./Device/led.o 

C_DEPS += \
./Device/ES8388.d \
./Device/beep.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\16-蜂鸣器\WCH-CH32V307VCT6-BEEP\Debug" -I"D:\WorkingProject\Module-Drive\16-蜂鸣器\WCH-CH32V307VCT6-BEEP\Driver" -I"D:\WorkingProject\Module-Drive\16-蜂鸣器\WCH-CH32V307VCT6-BEEP\Device" -I"D:\WorkingProject\Module-Drive\16-蜂鸣器\WCH-CH32V307VCT6-BEEP\Core" -I"D:\WorkingProject\Module-Drive\16-蜂鸣器\WCH-CH32V307VCT6-BEEP\User" -I"D:\WorkingProject\Module-Drive\16-蜂鸣器\WCH-CH32V307VCT6-BEEP\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

