################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

OBJS += \
./Core/core_riscv.o 

C_DEPS += \
./Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\09-WIFI模块（待定\WCH-CH32V307VCT6-ESP8266\Debug" -I"D:\WorkingProject\Module-Drive\09-WIFI模块（待定\WCH-CH32V307VCT6-ESP8266\Driver" -I"D:\WorkingProject\Module-Drive\09-WIFI模块（待定\WCH-CH32V307VCT6-ESP8266\Device" -I"D:\WorkingProject\Module-Drive\09-WIFI模块（待定\WCH-CH32V307VCT6-ESP8266\Core" -I"D:\WorkingProject\Module-Drive\09-WIFI模块（待定\WCH-CH32V307VCT6-ESP8266\User" -I"D:\WorkingProject\Module-Drive\09-WIFI模块（待定\WCH-CH32V307VCT6-ESP8266\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

