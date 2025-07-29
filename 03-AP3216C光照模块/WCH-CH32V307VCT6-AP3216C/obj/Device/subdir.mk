################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/AP3216C.c \
../Device/ES8388.c \
../Device/led.c 

OBJS += \
./Device/AP3216C.o \
./Device/ES8388.o \
./Device/led.o 

C_DEPS += \
./Device/AP3216C.d \
./Device/ES8388.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\03-AP3216C光照模块\WCH-CH32V307VCT6-AP3216C\Debug" -I"D:\WorkingProject\Module-Drive\03-AP3216C光照模块\WCH-CH32V307VCT6-AP3216C\Driver" -I"D:\WorkingProject\Module-Drive\03-AP3216C光照模块\WCH-CH32V307VCT6-AP3216C\Device" -I"D:\WorkingProject\Module-Drive\03-AP3216C光照模块\WCH-CH32V307VCT6-AP3216C\Core" -I"D:\WorkingProject\Module-Drive\03-AP3216C光照模块\WCH-CH32V307VCT6-AP3216C\User" -I"D:\WorkingProject\Module-Drive\03-AP3216C光照模块\WCH-CH32V307VCT6-AP3216C\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

