################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/FrontLayer/msg_parsing.c 

OBJS += \
./Core/FrontLayer/msg_parsing.o 

C_DEPS += \
./Core/FrontLayer/msg_parsing.d 


# Each subdirectory must supply rules for building sources it contributes
Core/FrontLayer/msg_parsing.o: ../Core/FrontLayer/msg_parsing.c Core/FrontLayer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Melle/SoftOnt/softwareont/swont_ide/Core/ub_lib" -I../Core/LogicLayer -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-FrontLayer

clean-Core-2f-FrontLayer:
	-$(RM) ./Core/FrontLayer/msg_parsing.cyclo ./Core/FrontLayer/msg_parsing.d ./Core/FrontLayer/msg_parsing.o ./Core/FrontLayer/msg_parsing.su

.PHONY: clean-Core-2f-FrontLayer

