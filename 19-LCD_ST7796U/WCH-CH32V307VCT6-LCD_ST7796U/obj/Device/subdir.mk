################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/ES8388.c \
../Device/lcd.c \
../Device/led.c \
../Device/touch-XPT2046.c 

OBJS += \
./Device/ES8388.o \
./Device/lcd.o \
./Device/led.o \
./Device/touch-XPT2046.o 

C_DEPS += \
./Device/ES8388.d \
./Device/lcd.d \
./Device/led.d \
./Device/touch-XPT2046.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Debug" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Core" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\User" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Device" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

