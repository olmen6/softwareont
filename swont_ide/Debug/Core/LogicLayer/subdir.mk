################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LogicLayer/logic.c 

OBJS += \
./Core/LogicLayer/logic.o 

C_DEPS += \
./Core/LogicLayer/logic.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LogicLayer/logic.o: ../Core/LogicLayer/logic.c Core/LogicLayer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Melle/SoftOnt/softwareont/swont_ide/Core/ub_lib" -I../Core/APILayer -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-LogicLayer

clean-Core-2f-LogicLayer:
	-$(RM) ./Core/LogicLayer/logic.cyclo ./Core/LogicLayer/logic.d ./Core/LogicLayer/logic.o ./Core/LogicLayer/logic.su

.PHONY: clean-Core-2f-LogicLayer

