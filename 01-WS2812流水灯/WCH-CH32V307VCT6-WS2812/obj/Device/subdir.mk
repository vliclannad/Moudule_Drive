################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/led.c \
../Device/ws2812-B.c 

OBJS += \
./Device/ES8388.o \
./Device/led.o \
./Device/ws2812-B.o 

C_DEPS += \
./Device/ES8388.d \
./Device/led.d \
./Device/ws2812-B.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\01-WS2812流水灯\WCH-CH32V307VCT6-WS2812\Debug" -I"D:\WorkingProject\Module-Drive\01-WS2812流水灯\WCH-CH32V307VCT6-WS2812\Driver" -I"D:\WorkingProject\Module-Drive\01-WS2812流水灯\WCH-CH32V307VCT6-WS2812\Device" -I"D:\WorkingProject\Module-Drive\01-WS2812流水灯\WCH-CH32V307VCT6-WS2812\Core" -I"D:\WorkingProject\Module-Drive\01-WS2812流水灯\WCH-CH32V307VCT6-WS2812\User" -I"D:\WorkingProject\Module-Drive\01-WS2812流水灯\WCH-CH32V307VCT6-WS2812\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

