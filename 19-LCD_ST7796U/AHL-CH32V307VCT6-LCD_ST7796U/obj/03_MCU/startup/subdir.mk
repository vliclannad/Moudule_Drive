################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../03_MCU/startup/system_ch32v30x.c 

S_UPPER_SRCS += \
../03_MCU/startup/startup_ch32v30x.S 

OBJS += \
./03_MCU/startup/startup_ch32v30x.o \
./03_MCU/startup/system_ch32v30x.o 

S_UPPER_DEPS += \
./03_MCU/startup/startup_ch32v30x.d 

C_DEPS += \
./03_MCU/startup/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
03_MCU/startup/%.o: ../03_MCU/startup/%.S
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -x assembler -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
03_MCU/startup/%.o: ../03_MCU/startup/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\02_CPU" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\Linker_file" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\MCU_drivers" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\03_MCU\startup" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\04_GEC" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\05_UserBoard" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\06_SoftComponent" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\AHL-CH32V307-LCD_ST7796U\07_AppPrg" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

