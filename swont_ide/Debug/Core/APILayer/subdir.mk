################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/APILayer/api_draw_functies.c 

OBJS += \
./Core/APILayer/api_draw_functies.o 

C_DEPS += \
./Core/APILayer/api_draw_functies.d 


# Each subdirectory must supply rules for building sources it contributes
Core/APILayer/%.o Core/APILayer/%.su Core/APILayer/%.cyclo: ../Core/APILayer/%.c Core/APILayer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Melle/SoftOnt/softwareont/swont_ide/Core/ub_lib" -I"C:/Users/Melle/SoftOnt/softwareont/swont_ide" -I/Core/APILayer/ -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-APILayer

clean-Core-2f-APILayer:
	-$(RM) ./Core/APILayer/api_draw_functies.cyclo ./Core/APILayer/api_draw_functies.d ./Core/APILayer/api_draw_functies.o ./Core/APILayer/api_draw_functies.su

.PHONY: clean-Core-2f-APILayer

