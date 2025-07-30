################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\13-����ģ�飨����\WCH-CH32V307VCT6-BLE\Debug" -I"D:\WorkingProject\Module-Drive\13-����ģ�飨����\WCH-CH32V307VCT6-BLE\Core" -I"D:\WorkingProject\Module-Drive\13-����ģ�飨����\WCH-CH32V307VCT6-BLE\User" -I"D:\WorkingProject\Module-Drive\13-����ģ�飨����\WCH-CH32V307VCT6-BLE\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\13-����ģ�飨����\WCH-CH32V307VCT6-BLE\Device" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

