################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/sdio.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/sdio.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/sdio.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\WorkingProject\Module-Drive\11-TF卡\WCH-CH32V307VCT6-SD\Debug" -I"D:\WorkingProject\Module-Drive\11-TF卡\WCH-CH32V307VCT6-SD\Core" -I"D:\WorkingProject\Module-Drive\11-TF卡\WCH-CH32V307VCT6-SD\User" -I"D:\WorkingProject\Module-Drive\11-TF卡\WCH-CH32V307VCT6-SD\Peripheral\inc" -I"D:\WorkingProject\Module-Drive\11-TF卡\WCH-CH32V307VCT6-SD\Device" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

