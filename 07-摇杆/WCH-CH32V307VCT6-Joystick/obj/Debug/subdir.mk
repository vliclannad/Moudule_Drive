################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
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
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\07-ҡ��\WCH-CH32V307VCT6-Joystick\Debug" -I"D:\WorkingProject\Module-Drive\07-ҡ��\WCH-CH32V307VCT6-Joystick\Driver" -I"D:\WorkingProject\Module-Drive\07-ҡ��\WCH-CH32V307VCT6-Joystick\Device" -I"D:\WorkingProject\Module-Drive\07-ҡ��\WCH-CH32V307VCT6-Joystick\Core" -I"D:\WorkingProject\Module-Drive\07-ҡ��\WCH-CH32V307VCT6-Joystick\User" -I"D:\WorkingProject\Module-Drive\07-ҡ��\WCH-CH32V307VCT6-Joystick\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

