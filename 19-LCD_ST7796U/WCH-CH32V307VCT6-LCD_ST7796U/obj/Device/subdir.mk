################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/lcd.c \
../Device/touch-XPT2046.c 

OBJS += \
./Device/lcd.o \
./Device/touch-XPT2046.o 

C_DEPS += \
./Device/lcd.d \
./Device/touch-XPT2046.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Debug" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Core" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\User" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Peripheral\inc" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Device" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\19-LCD_ST7796U\WCH-CH32V307VCT6-LCD_ST7796U\Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

