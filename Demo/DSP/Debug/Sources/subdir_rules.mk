################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Sources/%.obj: ../Sources/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/ti/C2000Ware_4_01_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/C2000Ware_4_01_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/C2000Ware_4_01_00_00/device_support/f2837xd/headers/include" --include_path="D:/Subjects/TCC/Comm_device/F28379D/DSP_TI_Tests-master" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/include" --advice:performance=all --define=_LAUNCHXL_F28379D --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Sources/$(basename $(<F)).d_raw" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources/main.obj: ../Sources/main.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/ti/C2000Ware_4_01_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/C2000Ware_4_01_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/C2000Ware_4_01_00_00/device_support/f2837xd/headers/include" --include_path="D:/Subjects/TCC/Comm_device/F28379D/DSP_TI_Tests-master" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.12.0.STS/include" --advice:performance=all --define=_LAUNCHXL_F28379D --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Sources/$(basename $(<F)).d_raw" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


