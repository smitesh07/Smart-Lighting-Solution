################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/port.obj: C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/port.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
<<<<<<< HEAD
	"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include" --include_path="C:/Users/ashis/Documents/GitHub/Smart-Lighting-Solution/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power="all" -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
=======
	"D:/Installed_Softwares/ti/ccs900/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="D:/Installed_Softwares/ti/ccs900/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include" --include_path="D:/Git_repo/Smart-Lighting-Solution/FreeRTOS" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/include" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power="all" -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
>>>>>>> a2c5ca3c59bf9d796bdb182fb08d646cafd00b14
	@echo 'Finished building: "$<"'
	@echo ' '

third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/portasm.obj: C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/portasm.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
<<<<<<< HEAD
	"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include" --include_path="C:/Users/ashis/Documents/GitHub/Smart-Lighting-Solution/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power="all" -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
=======
	"D:/Installed_Softwares/ti/ccs900/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="D:/Installed_Softwares/ti/ccs900/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include" --include_path="D:/Git_repo/Smart-Lighting-Solution/FreeRTOS" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/include" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS" --include_path="D:/Installed_Softwares/ti/TivaWare_C_Series-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power="all" -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
>>>>>>> a2c5ca3c59bf9d796bdb182fb08d646cafd00b14
	@echo 'Finished building: "$<"'
	@echo ' '


