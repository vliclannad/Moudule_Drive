################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../07_AppPrg/isr.c \
../07_AppPrg/main.c 

OBJS += \
./07_AppPrg/isr.o \
./07_AppPrg/main.o 

C_DEPS += \
./07_AppPrg/isr.d \
./07_AppPrg/main.d 


# Each subdirectory must supply rules for building sources it contributes
07_AppPrg/%.o: ../07_AppPrg/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\02_CPU" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\Linker_file" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\MCU_drivers" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\startup" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\04_GEC" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\05_UserBoard" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\06_SoftComponent" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\07_AppPrg" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

