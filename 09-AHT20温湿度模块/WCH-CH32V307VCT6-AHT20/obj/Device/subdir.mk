################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/AHT20.c \
../Device/ES8388.c \
../Device/led.c 

OBJS += \
./Device/AHT20.o \
./Device/ES8388.o \
./Device/led.o 

C_DEPS += \
./Device/AHT20.d \
./Device/ES8388.d \
./Device/led.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\09-AHT20��ʪ��ģ��\WCH-CH32V307VCT6-AHT20\Debug" -I"D:\WorkingProject\Module-Drive\09-AHT20��ʪ��ģ��\WCH-CH32V307VCT6-AHT20\Core" -I"D:\WorkingProject\Module-Drive\09-AHT20��ʪ��ģ��\WCH-CH32V307VCT6-AHT20\User" -I"D:\WorkingProject\Module-Drive\09-AHT20��ʪ��ģ��\WCH-CH32V307VCT6-AHT20\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\09-AHT20��ʪ��ģ��\WCH-CH32V307VCT6-AHT20\Device" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

