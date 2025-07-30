################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../03_MCU/MCU_drivers/adc.c \
../03_MCU/MCU_drivers/flash.c \
../03_MCU/MCU_drivers/gpio.c \
../03_MCU/MCU_drivers/spi.c \
../03_MCU/MCU_drivers/timer.c \
../03_MCU/MCU_drivers/uart.c \
../03_MCU/MCU_drivers/wdog.c 

OBJS += \
./03_MCU/MCU_drivers/adc.o \
./03_MCU/MCU_drivers/flash.o \
./03_MCU/MCU_drivers/gpio.o \
./03_MCU/MCU_drivers/spi.o \
./03_MCU/MCU_drivers/timer.o \
./03_MCU/MCU_drivers/uart.o \
./03_MCU/MCU_drivers/wdog.o 

C_DEPS += \
./03_MCU/MCU_drivers/adc.d \
./03_MCU/MCU_drivers/flash.d \
./03_MCU/MCU_drivers/gpio.d \
./03_MCU/MCU_drivers/spi.d \
./03_MCU/MCU_drivers/timer.d \
./03_MCU/MCU_drivers/uart.d \
./03_MCU/MCU_drivers/wdog.d 


# Each subdirectory must supply rules for building sources it contributes
03_MCU/MCU_drivers/%.o: ../03_MCU/MCU_drivers/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\02_CPU" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\Linker_file" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\MCU_drivers" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\startup" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\04_GEC" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\05_UserBoard" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\06_SoftComponent" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\07_AppPrg" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

