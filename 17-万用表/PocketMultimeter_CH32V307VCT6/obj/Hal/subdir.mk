################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hal/pm_adc.c \
../Hal/pm_bjt.c \
../Hal/pm_capacitor.c \
../Hal/pm_ch9141.c \
../Hal/pm_diode.c \
../Hal/pm_display.c \
../Hal/pm_key.c \
../Hal/pm_lcd.c \
../Hal/pm_measure.c \
../Hal/pm_probe.c \
../Hal/pm_resistance.c \
../Hal/pm_timer.c 

OBJS += \
./Hal/pm_adc.o \
./Hal/pm_bjt.o \
./Hal/pm_capacitor.o \
./Hal/pm_ch9141.o \
./Hal/pm_diode.o \
./Hal/pm_display.o \
./Hal/pm_key.o \
./Hal/pm_lcd.o \
./Hal/pm_measure.o \
./Hal/pm_probe.o \
./Hal/pm_resistance.o \
./Hal/pm_timer.o 

C_DEPS += \
./Hal/pm_adc.d \
./Hal/pm_bjt.d \
./Hal/pm_capacitor.d \
./Hal/pm_ch9141.d \
./Hal/pm_diode.d \
./Hal/pm_display.d \
./Hal/pm_key.d \
./Hal/pm_lcd.d \
./Hal/pm_measure.d \
./Hal/pm_probe.d \
./Hal/pm_resistance.d \
./Hal/pm_timer.d 


# Each subdirectory must supply rules for building sources it contributes
Hal/%.o: ../Hal/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DBOARD_CHITU -I"D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\Debug" -I"D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\Core" -I"D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\User" -I"D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\Hal\include" -I"D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\Device" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

