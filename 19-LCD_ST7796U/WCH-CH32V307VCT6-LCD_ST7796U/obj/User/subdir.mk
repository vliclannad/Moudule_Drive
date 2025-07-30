################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
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
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Debug" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Core" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\User" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Device" -I"D:\WorkingProject\Module-Drive\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

