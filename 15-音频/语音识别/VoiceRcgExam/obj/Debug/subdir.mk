################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\Debug" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\Core" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\User" -I"E:\02-��Ŀ\13-RISCVʵ����\ģ������\VoiceRcgExam\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

