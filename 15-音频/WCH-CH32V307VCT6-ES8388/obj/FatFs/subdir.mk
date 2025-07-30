################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FatFs/diskio.c \
../FatFs/ff.c \
../FatFs/ffsystem.c \
../FatFs/ffunicode.c 

OBJS += \
./FatFs/diskio.o \
./FatFs/ff.o \
./FatFs/ffsystem.o \
./FatFs/ffunicode.o 

C_DEPS += \
./FatFs/diskio.d \
./FatFs/ff.d \
./FatFs/ffsystem.d \
./FatFs/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
FatFs/%.o: ../FatFs/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Debug" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Core" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\User" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\FatFs" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Device" -I"D:\WorkingProject\Module-Drive\15-音频\WCH-CH32V307VCT6-ES8388\Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

