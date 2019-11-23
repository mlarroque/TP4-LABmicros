################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/FSM.c \
../source/SegmentDisplay.c \
../source/SysTick.c \
../source/dataBase.c \
../source/display.c \
../source/doorManagement.c \
../source/encoder.c \
../source/encoderDecoder.c \
../source/encoderHAL.c \
../source/gpio.c \
../source/magnetCardDecode.c \
../source/magnetCardLector.c \
../source/magnetLectorHAL.c \
../source/queue.c \
../source/stateAddingUser.c \
../source/stateAddingUserPIN.c \
../source/stateAdminMode.c \
../source/stateBlocked.c \
../source/stateChangeIntensity.c \
../source/stateChangingPIN.c \
../source/stateMenu.c \
../source/stateReceivingID.c \
../source/stateReceivingPIN.c \
../source/stateRemovingUser.c \
../source/stateUserApproved.c \
../source/timer.c \
../source/timer_queue.c 

OBJS += \
./source/App.o \
./source/FSM.o \
./source/SegmentDisplay.o \
./source/SysTick.o \
./source/dataBase.o \
./source/display.o \
./source/doorManagement.o \
./source/encoder.o \
./source/encoderDecoder.o \
./source/encoderHAL.o \
./source/gpio.o \
./source/magnetCardDecode.o \
./source/magnetCardLector.o \
./source/magnetLectorHAL.o \
./source/queue.o \
./source/stateAddingUser.o \
./source/stateAddingUserPIN.o \
./source/stateAdminMode.o \
./source/stateBlocked.o \
./source/stateChangeIntensity.o \
./source/stateChangingPIN.o \
./source/stateMenu.o \
./source/stateReceivingID.o \
./source/stateReceivingPIN.o \
./source/stateRemovingUser.o \
./source/stateUserApproved.o \
./source/timer.o \
./source/timer_queue.o 

C_DEPS += \
./source/App.d \
./source/FSM.d \
./source/SegmentDisplay.d \
./source/SysTick.d \
./source/dataBase.d \
./source/display.d \
./source/doorManagement.d \
./source/encoder.d \
./source/encoderDecoder.d \
./source/encoderHAL.d \
./source/gpio.d \
./source/magnetCardDecode.d \
./source/magnetCardLector.d \
./source/magnetLectorHAL.d \
./source/queue.d \
./source/stateAddingUser.d \
./source/stateAddingUserPIN.d \
./source/stateAdminMode.d \
./source/stateBlocked.d \
./source/stateChangeIntensity.d \
./source/stateChangingPIN.d \
./source/stateMenu.d \
./source/stateReceivingID.d \
./source/stateReceivingPIN.d \
./source/stateRemovingUser.d \
./source/stateUserApproved.d \
./source/timer.d \
./source/timer_queue.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/dataBase.o: ../source/dataBase.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"source/dataBase.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/stateReceivingPIN.o: ../source/stateReceivingPIN.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"source/stateReceivingPIN.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


