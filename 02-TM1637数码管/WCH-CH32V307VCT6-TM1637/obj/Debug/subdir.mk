################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\02-TM1637数码管\WCH-CH32V307VCT6-TM1637\Debug" -I"D:\WorkingProject\Module-Drive\02-TM1637数码管\WCH-CH32V307VCT6-TM1637\Driver" -I"D:\WorkingProject\Module-Drive\02-TM1637数码管\WCH-CH32V307VCT6-TM1637\Device" -I"D:\WorkingProject\Module-Drive\02-TM1637数码管\WCH-CH32V307VCT6-TM1637\Core" -I"D:\WorkingProject\Module-Drive\02-TM1637数码管\WCH-CH32V307VCT6-TM1637\User" -I"D:\WorkingProject\Module-Drive\02-TM1637数码管\WCH-CH32V307VCT6-TM1637\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

