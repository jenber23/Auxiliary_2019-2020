################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f072rbtx.s 

S_DEPS += \
./Core/Startup/startup_stm32f072rbtx.d 

OBJS += \
./Core/Startup/startup_stm32f072rbtx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32f072rbtx.o: ../Core/Startup/startup_stm32f072rbtx.s
	arm-none-eabi-gcc -mcpu=cortex-m0 -g3 -c -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32f072rbtx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

