################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/IIC.c \
../Driver/NVIC.c \
../Driver/TIM.c \
../Driver/user_usart.c 

OBJS += \
./Driver/IIC.o \
./Driver/NVIC.o \
./Driver/TIM.o \
./Driver/user_usart.o 

C_DEPS += \
./Driver/IIC.d \
./Driver/NVIC.d \
./Driver/TIM.d \
./Driver/user_usart.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/%.o: ../Driver/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Debug" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Driver" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Device" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Core" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\User" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

