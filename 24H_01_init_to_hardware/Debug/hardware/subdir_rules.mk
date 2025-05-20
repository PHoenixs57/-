################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
hardware/%.o: ../hardware/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/TI/code/2024H/2024H/24H_01_init_to_hardware" -I"D:/TI/code/2024H/2024H/24H_01_init_to_hardware/OLED" -I"D:/TI/code/2024H/2024H/24H_01_init_to_hardware/hardware" -I"D:/TI/code/2024H/2024H/24H_01_init_to_hardware/Sys" -I"D:/TI/code/2024H/2024H/24H_01_init_to_hardware/Debug" -I"C:/ti/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_01_00_03/source" -I"D:/TI/code/2024H/2024H/24H_01_init_to_hardware/pid" -gdwarf-3 -MMD -MP -MF"hardware/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


