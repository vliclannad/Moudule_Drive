################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

OBJS += \
./Core/core_riscv.o 

C_DEPS += \
./Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Debug" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Core" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\User" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Peripheral\inc" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Device" -I"E:\02-项目\13-RISCV实验箱\模块驱动\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

