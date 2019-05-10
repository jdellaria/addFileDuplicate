################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/addFileDuplicate.cpp \
../src/configurationFile.cpp \
../src/coopDB.cpp 

OBJS += \
./src/addFileDuplicate.o \
./src/configurationFile.o \
./src/coopDB.o 

CPP_DEPS += \
./src/addFileDuplicate.d \
./src/configurationFile.d \
./src/coopDB.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jdellaria/eclipse-workspace/DLiriumLib" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


