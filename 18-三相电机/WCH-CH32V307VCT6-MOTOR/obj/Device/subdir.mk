################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/keyboard.c \
../Device/led.c \
../Device/motor.c 

OBJS += \
./Device/keyboard.o \
./Device/led.o \
./Device/motor.o 

C_DEPS += \
./Device/keyboard.d \
./Device/led.d \
./Device/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\18-������\WCH-CH32V307VCT6-MOTOR\Debug" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\18-������\WCH-CH32V307VCT6-MOTOR\Driver" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\18-������\WCH-CH32V307VCT6-MOTOR\Device" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\18-������\WCH-CH32V307VCT6-MOTOR\Core" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\18-������\WCH-CH32V307VCT6-MOTOR\User" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\18-������\WCH-CH32V307VCT6-MOTOR\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

