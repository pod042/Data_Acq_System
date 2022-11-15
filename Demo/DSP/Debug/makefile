################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Sources/ADC.obj" \
"./Sources/EPWM.obj" \
"./Sources/GPIO.obj" \
"./Sources/SPI_logger.obj" \
"./Sources/interrupts.obj" \
"./Sources/main.obj" \
"./Sources/system.obj" \
"./Sources_TI/F2837xD_Adc.obj" \
"./Sources_TI/F2837xD_CodeStartBranch.obj" \
"./Sources_TI/F2837xD_DefaultISR.obj" \
"./Sources_TI/F2837xD_EPwm.obj" \
"./Sources_TI/F2837xD_GlobalVariableDefs.obj" \
"./Sources_TI/F2837xD_Gpio.obj" \
"./Sources_TI/F2837xD_Ipc.obj" \
"./Sources_TI/F2837xD_PieCtrl.obj" \
"./Sources_TI/F2837xD_PieVect.obj" \
"./Sources_TI/F2837xD_Spi.obj" \
"./Sources_TI/F2837xD_SysCtrl.obj" \
"./Sources_TI/F2837xD_usDelay.obj" \
"../2837x_RAM_lnk_cpu1.cmd" \
$(GEN_CMDS__FLAG) \
-lF2837xD_Headers_nonBIOS_cpu1.cmd \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Sources/subdir_vars.mk
-include Sources_TI/subdir_vars.mk
-include subdir_rules.mk
-include Sources/subdir_rules.mk
-include Sources_TI/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Progressive_Tests.out \

EXE_OUTPUTS__QUOTED += \
"Progressive_Tests.out" \

BIN_OUTPUTS += \
Progressive_Tests.hex \

BIN_OUTPUTS__QUOTED += \
"Progressive_Tests.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Progressive_Tests.out"

# Tool invocations
Progressive_Tests.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --advice:performance=all --define=_LAUNCHXL_F28379D --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"Progressive_Tests.map" --stack_size=0x200 --warn_sections -i"C:/ti/C2000Ware_4_01_00_00/device_support/f2837xd/common/cmd" -i"C:/ti/C2000Ware_4_01_00_00/device_support/f2837xd/headers/cmd" -i"D:/Subjects/TCC/Comm_device/F28379D/DSP_TI_Tests-master/Includes" -i"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/lib" -i"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Progressive_Tests_linkInfo.xml" --rom_model -o "Progressive_Tests.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Progressive_Tests.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/bin/hex2000" --diag_wrap=off -o "Progressive_Tests.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Sources\ADC.obj" "Sources\EPWM.obj" "Sources\GPIO.obj" "Sources\SPI_logger.obj" "Sources\interrupts.obj" "Sources\main.obj" "Sources\system.obj" "Sources_TI\F2837xD_Adc.obj" "Sources_TI\F2837xD_CodeStartBranch.obj" "Sources_TI\F2837xD_DefaultISR.obj" "Sources_TI\F2837xD_EPwm.obj" "Sources_TI\F2837xD_GlobalVariableDefs.obj" "Sources_TI\F2837xD_Gpio.obj" "Sources_TI\F2837xD_Ipc.obj" "Sources_TI\F2837xD_PieCtrl.obj" "Sources_TI\F2837xD_PieVect.obj" "Sources_TI\F2837xD_Spi.obj" "Sources_TI\F2837xD_SysCtrl.obj" "Sources_TI\F2837xD_usDelay.obj" 
	-$(RM) "Sources\ADC.d" "Sources\EPWM.d" "Sources\GPIO.d" "Sources\SPI_logger.d" "Sources\interrupts.d" "Sources\main.d" "Sources\system.d" "Sources_TI\F2837xD_Adc.d" "Sources_TI\F2837xD_DefaultISR.d" "Sources_TI\F2837xD_EPwm.d" "Sources_TI\F2837xD_GlobalVariableDefs.d" "Sources_TI\F2837xD_Gpio.d" "Sources_TI\F2837xD_Ipc.d" "Sources_TI\F2837xD_PieCtrl.d" "Sources_TI\F2837xD_PieVect.d" "Sources_TI\F2837xD_Spi.d" "Sources_TI\F2837xD_SysCtrl.d" 
	-$(RM) "Sources_TI\F2837xD_CodeStartBranch.d" "Sources_TI\F2837xD_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

