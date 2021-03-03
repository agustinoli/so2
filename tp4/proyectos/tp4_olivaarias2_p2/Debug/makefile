################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: tp4_olivaarias2_p2.axf

# Tool invocations
tp4_olivaarias2_p2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/agustinoli/Documents/MCUXpresso/workspace/CMSISv2p00_LPC17xx/Debug" -L"/home/agustinoli/Documents/MCUXpresso/workspace/CMSISv2p00_LPC17xx/Drivers/inc" -Xlinker -Map="tp4_olivaarias2_p2.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "tp4_olivaarias_Debug.ld" -o "tp4_olivaarias2_p2.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) tp4_olivaarias2_p2.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "tp4_olivaarias2_p2.axf"; # arm-none-eabi-objcopy -v -O binary "tp4_olivaarias2_p2.axf" "tp4_olivaarias2_p2.bin" ; # checksum -p LPC1769 -d "tp4_olivaarias2_p2.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
