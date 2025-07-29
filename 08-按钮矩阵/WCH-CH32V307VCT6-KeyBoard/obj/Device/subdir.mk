################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/keyboard.c \
../Device/led.c 

OBJS += \
./Device/ES8388.o \
./Device/keyboard.o \
./Device/led.o 

C_DEPS += \
./Device/ES8388.d \
./Device/keyboard.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Debug" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Driver" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Device" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Core" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\User" -I"D:\WorkingProject\Module-Drive\08-��ť����\WCH-CH32V307VCT6-KeyBoard\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

