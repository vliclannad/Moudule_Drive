################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/led.c \
../Device/wavplay.c 

OBJS += \
./Device/ES8388.o \
./Device/led.o \
./Device/wavplay.o 

C_DEPS += \
./Device/ES8388.d \
./Device/led.d \
./Device/wavplay.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Debug" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Core" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\User" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\FatFs" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Device" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

