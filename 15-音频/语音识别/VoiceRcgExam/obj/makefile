################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
VoiceRcgExam.hex \

SECONDARY_LIST += \
VoiceRcgExam.lst \

SECONDARY_SIZE += \
VoiceRcgExam.siz \


# 所有目标
all: VoiceRcgExam.elf secondary-outputs

# 工具调用
VoiceRcgExam.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -T "E:\02-项目\13-RISCV实验箱\模块驱动\VoiceRcgExam\User\Link.ld" -nostartfiles -Xlinker --gc-sections -L"E:\02-项目\13-RISCV实验箱\模块驱动\VoiceRcgExam\User" -Wl,-Map,"VoiceRcgExam.map" --specs=nano.specs --specs=nosys.specs -o "VoiceRcgExam.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
VoiceRcgExam.hex: VoiceRcgExam.elf
	@	riscv-none-embed-objcopy -O ihex "VoiceRcgExam.elf"  "VoiceRcgExam.hex"
	@	@
VoiceRcgExam.lst: VoiceRcgExam.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "VoiceRcgExam.elf" > "VoiceRcgExam.lst"
	@	@
VoiceRcgExam.siz: VoiceRcgExam.elf
	@	riscv-none-embed-size --format=berkeley "VoiceRcgExam.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) VoiceRcgExam.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
