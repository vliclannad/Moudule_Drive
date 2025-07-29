################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
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
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\02-TM1637�����\WCH-CH32V307VCT6-TM1637\Debug" -I"D:\WorkingProject\Module-Drive\02-TM1637�����\WCH-CH32V307VCT6-TM1637\Driver" -I"D:\WorkingProject\Module-Drive\02-TM1637�����\WCH-CH32V307VCT6-TM1637\Device" -I"D:\WorkingProject\Module-Drive\02-TM1637�����\WCH-CH32V307VCT6-TM1637\Core" -I"D:\WorkingProject\Module-Drive\02-TM1637�����\WCH-CH32V307VCT6-TM1637\User" -I"D:\WorkingProject\Module-Drive\02-TM1637�����\WCH-CH32V307VCT6-TM1637\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

