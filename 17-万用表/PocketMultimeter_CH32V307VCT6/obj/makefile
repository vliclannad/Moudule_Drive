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
-include Hal/subdir.mk
-include Device/subdir.mk
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
PocketMultimeter_CH32V307VCT6.hex \

SECONDARY_LIST += \
PocketMultimeter_CH32V307VCT6.lst \

SECONDARY_SIZE += \
PocketMultimeter_CH32V307VCT6.siz \


# 所有目标
all: PocketMultimeter_CH32V307VCT6.elf secondary-outputs

# 工具调用
PocketMultimeter_CH32V307VCT6.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "D:\WorkingProject\Module-Drive\17-万用表\PocketMultimeter_CH32V307VCT6\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"PocketMultimeter_CH32V307VCT6.map" --specs=nano.specs -u _printf_float --specs=nosys.specs -o "PocketMultimeter_CH32V307VCT6.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
PocketMultimeter_CH32V307VCT6.hex: PocketMultimeter_CH32V307VCT6.elf
	@	riscv-none-embed-objcopy -O ihex "PocketMultimeter_CH32V307VCT6.elf"  "PocketMultimeter_CH32V307VCT6.hex"
	@	@
PocketMultimeter_CH32V307VCT6.lst: PocketMultimeter_CH32V307VCT6.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "PocketMultimeter_CH32V307VCT6.elf" > "PocketMultimeter_CH32V307VCT6.lst"
	@	@
PocketMultimeter_CH32V307VCT6.siz: PocketMultimeter_CH32V307VCT6.elf
	@	riscv-none-embed-size --format=berkeley "PocketMultimeter_CH32V307VCT6.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) PocketMultimeter_CH32V307VCT6.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
