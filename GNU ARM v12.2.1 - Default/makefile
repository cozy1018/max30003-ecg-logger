################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include simplicity_sdk_2025.6.1/platform/service/udelay/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/sleeptimer/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/sl_main/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/memory_manager/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/iostream/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/interrupt_manager/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/device_manager/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/device_manager/devices/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/device_manager/clocks/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/device_init/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/service/clock_manager/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/peripheral/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/emlib/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/emdrv/dmadrv/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/driver/gpio/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/driver/debug/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/common/src/subdir.mk
-include simplicity_sdk_2025.6.1/platform/Device/SiliconLabs/EFR32MG24/Source/subdir.mk
-include simplicity_sdk_2025.6.1/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_eusart/subdir.mk
-include simplicity_sdk_2025.6.1/hardware/driver/configuration_over_swo/src/subdir.mk
-include simplicity_sdk_2025.6.1/hardware/board/src/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := iostream_eusart_baremetal_new
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: iostream_eusart_baremetal_new.axf

# Tool invocations
iostream_eusart_baremetal_new.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "/Users/hsinwen/SimplicityStudio/v5_workspace/iostream_eusart_baremetal_new/autogen/linkerfile.ld" -fno-lto -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="iostream_eusart_baremetal_new.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o iostream_eusart_baremetal_new.axf -Wl,--start-group "./app.o" "./app_iostream_eusart.o" "./main.o" "./max30003.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_eusart_instances.o" "./simplicity_sdk_2025.6.1/hardware/board/src/sl_board_control_gpio.o" "./simplicity_sdk_2025.6.1/hardware/board/src/sl_board_init.o" "./simplicity_sdk_2025.6.1/hardware/driver/configuration_over_swo/src/sl_cos.o" "./simplicity_sdk_2025.6.1/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_eusart/sl_mx25_flash_shutdown.o" "./simplicity_sdk_2025.6.1/platform/Device/SiliconLabs/EFR32MG24/Source/startup_efr32mg24.o" "./simplicity_sdk_2025.6.1/platform/Device/SiliconLabs/EFR32MG24/Source/system_efr32mg24.o" "./simplicity_sdk_2025.6.1/platform/common/src/sl_assert.o" "./simplicity_sdk_2025.6.1/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2025.6.1/platform/common/src/sl_slist.o" "./simplicity_sdk_2025.6.1/platform/common/src/sl_string.o" "./simplicity_sdk_2025.6.1/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2025.6.1/platform/driver/debug/src/sl_debug_swo.o" "./simplicity_sdk_2025.6.1/platform/driver/gpio/src/sl_gpio.o" "./simplicity_sdk_2025.6.1/platform/emdrv/dmadrv/src/dmadrv.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_burtc.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_cmu.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_emu.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_eusart.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_gpio.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_ldma.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_msc.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_prs.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_system.o" "./simplicity_sdk_2025.6.1/platform/emlib/src/em_timer.o" "./simplicity_sdk_2025.6.1/platform/peripheral/src/sl_hal_eusart.o" "./simplicity_sdk_2025.6.1/platform/peripheral/src/sl_hal_gpio.o" "./simplicity_sdk_2025.6.1/platform/peripheral/src/sl_hal_prs.o" "./simplicity_sdk_2025.6.1/platform/peripheral/src/sl_hal_sysrtc.o" "./simplicity_sdk_2025.6.1/platform/service/clock_manager/src/sl_clock_manager.o" "./simplicity_sdk_2025.6.1/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o" "./simplicity_sdk_2025.6.1/platform/service/clock_manager/src/sl_clock_manager_init.o" "./simplicity_sdk_2025.6.1/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o" "./simplicity_sdk_2025.6.1/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./simplicity_sdk_2025.6.1/platform/service/device_init/src/sl_device_init_emu_s2.o" "./simplicity_sdk_2025.6.1/platform/service/device_manager/clocks/sl_device_clock_efr32xg24.o" "./simplicity_sdk_2025.6.1/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg24.o" "./simplicity_sdk_2025.6.1/platform/service/device_manager/src/sl_device_clock.o" "./simplicity_sdk_2025.6.1/platform/service/device_manager/src/sl_device_gpio.o" "./simplicity_sdk_2025.6.1/platform/service/device_manager/src/sl_device_peripheral.o" "./simplicity_sdk_2025.6.1/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o" "./simplicity_sdk_2025.6.1/platform/service/iostream/src/sl_iostream.o" "./simplicity_sdk_2025.6.1/platform/service/iostream/src/sl_iostream_eusart.o" "./simplicity_sdk_2025.6.1/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./simplicity_sdk_2025.6.1/platform/service/iostream/src/sl_iostream_uart.o" "./simplicity_sdk_2025.6.1/platform/service/memory_manager/src/sl_memory_manager_region.o" "./simplicity_sdk_2025.6.1/platform/service/sl_main/src/sl_main_init.o" "./simplicity_sdk_2025.6.1/platform/service/sl_main/src/sl_main_init_memory.o" "./simplicity_sdk_2025.6.1/platform/service/sl_main/src/sl_main_process_action.o" "./simplicity_sdk_2025.6.1/platform/service/sleeptimer/src/sl_sleeptimer.o" "./simplicity_sdk_2025.6.1/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./simplicity_sdk_2025.6.1/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o" "./simplicity_sdk_2025.6.1/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./simplicity_sdk_2025.6.1/platform/service/udelay/src/sl_udelay.o" "./simplicity_sdk_2025.6.1/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: iostream_eusart_baremetal_new.out'
	arm-none-eabi-objcopy "iostream_eusart_baremetal_new.axf" "iostream_eusart_baremetal_new.out"
	@echo ' '

	@echo 'Building hex file: iostream_eusart_baremetal_new.hex'
	arm-none-eabi-objcopy -O ihex "iostream_eusart_baremetal_new.axf" "iostream_eusart_baremetal_new.hex"
	@echo ' '

	@echo 'Building bin file: iostream_eusart_baremetal_new.bin'
	arm-none-eabi-objcopy -O binary "iostream_eusart_baremetal_new.axf" "iostream_eusart_baremetal_new.bin"
	@echo ' '

	@echo 'Building s37 file: iostream_eusart_baremetal_new.s37'
	arm-none-eabi-objcopy -O srec "iostream_eusart_baremetal_new.axf" "iostream_eusart_baremetal_new.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "iostream_eusart_baremetal_new.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) iostream_eusart_baremetal_new.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
