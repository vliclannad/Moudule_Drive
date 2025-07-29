################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/CS100A.c \
../Device/ES8388.c \
../Device/led.c 

OBJS += \
./Device/CS100A.o \
./Device/ES8388.o \
./Device/led.o 

C_DEPS += \
./Device/CS100A.d \
./Device/ES8388.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\04-CS100A超声波模块\WCH-CH32V307VCT6-CS100A\Debug" -I"D:\WorkingProject\Module-Drive\04-CS100A超声波模块\WCH-CH32V307VCT6-CS100A\Driver" -I"D:\WorkingProject\Module-Drive\04-CS100A超声波模块\WCH-CH32V307VCT6-CS100A\Device" -I"D:\WorkingProject\Module-Drive\04-CS100A超声波模块\WCH-CH32V307VCT6-CS100A\Core" -I"D:\WorkingProject\Module-Drive\04-CS100A超声波模块\WCH-CH32V307VCT6-CS100A\User" -I"D:\WorkingProject\Module-Drive\04-CS100A超声波模块\WCH-CH32V307VCT6-CS100A\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

