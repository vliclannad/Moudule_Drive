################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/adlx345.c \
../Device/led.c 

OBJS += \
./Device/ES8388.o \
./Device/adlx345.o \
./Device/led.o 

C_DEPS += \
./Device/ES8388.d \
./Device/adlx345.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\05-ADXL345加速度模块\WCH-CH32V307VCT6-ADXL345\Debug" -I"D:\WorkingProject\Module-Drive\05-ADXL345加速度模块\WCH-CH32V307VCT6-ADXL345\Driver" -I"D:\WorkingProject\Module-Drive\05-ADXL345加速度模块\WCH-CH32V307VCT6-ADXL345\Device" -I"D:\WorkingProject\Module-Drive\05-ADXL345加速度模块\WCH-CH32V307VCT6-ADXL345\Core" -I"D:\WorkingProject\Module-Drive\05-ADXL345加速度模块\WCH-CH32V307VCT6-ADXL345\User" -I"D:\WorkingProject\Module-Drive\05-ADXL345加速度模块\WCH-CH32V307VCT6-ADXL345\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

