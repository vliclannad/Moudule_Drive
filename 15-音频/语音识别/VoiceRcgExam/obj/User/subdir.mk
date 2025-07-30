################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Get_Data.c \
../User/ch32v30x_it.c \
../User/es8388.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/Get_Data.o \
./User/ch32v30x_it.o \
./User/es8388.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/Get_Data.d \
./User/ch32v30x_it.d \
./User/es8388.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\Debug" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\Core" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\User" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

