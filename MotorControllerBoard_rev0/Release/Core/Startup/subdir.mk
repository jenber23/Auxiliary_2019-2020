################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f072rbtx.s 

OBJS += \
./Core/Startup/startup_stm32f072rbtx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m0 -c -I"C:/Users/m37/Google Drive/Solar Gators/Auxiliary_2019-2020/MotorControllerBoard_rev0/Core/Src/subsystem-can-driver" -x assembler-with-cpp --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"
