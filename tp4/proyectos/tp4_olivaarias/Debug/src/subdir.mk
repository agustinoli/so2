################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x_bkp.c \
../src/heap_4.c \
../src/list.c \
../src/port.c \
../src/queue.c \
../src/tasks.c \
../src/timers.c \
../src/tp4_olivaarias.c \
../src/trcKernelPort.c \
../src/trcSnapshotRecorder.c \
../src/trcStreamingRecorder.c 

OBJS += \
./src/cr_startup_lpc175x_6x_bkp.o \
./src/heap_4.o \
./src/list.o \
./src/port.o \
./src/queue.o \
./src/tasks.o \
./src/timers.o \
./src/tp4_olivaarias.o \
./src/trcKernelPort.o \
./src/trcSnapshotRecorder.o \
./src/trcStreamingRecorder.o 

C_DEPS += \
./src/cr_startup_lpc175x_6x_bkp.d \
./src/heap_4.d \
./src/list.d \
./src/port.d \
./src/queue.d \
./src/tasks.d \
./src/timers.d \
./src/tp4_olivaarias.d \
./src/trcKernelPort.d \
./src/trcSnapshotRecorder.d \
./src/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"/home/agustinoli/Documents/MCUXpresso/workspace/tp4_olivaarias/inc" -I"/home/agustinoli/Documents/MCUXpresso/workspace/CMSISv2p00_LPC17xx/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


