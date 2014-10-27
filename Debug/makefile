################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
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
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
libadxl345.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: libadxl345.a secondary-outputs

# Tool invocations
libadxl345.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR Archiver'
	avr-ar -r  "libadxl345.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

libadxl345.lss: libadxl345.a
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S libadxl345.a  >"libadxl345.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: libadxl345.a
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega2560 libadxl345.a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(ARCHIVES)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) libadxl345.a
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
