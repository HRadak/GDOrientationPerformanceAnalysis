################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../io/convert.cpp \
../io/ioconfigfile.cpp \
../io/iodatafile.cpp \
../io/iodirectory.cpp \
../io/iofile.cpp \
../io/iooption.cpp \
../io/iosectionkeyfile.cpp \
../io/ioserial.cpp 

OBJS += \
./io/convert.o \
./io/ioconfigfile.o \
./io/iodatafile.o \
./io/iodirectory.o \
./io/iofile.o \
./io/iooption.o \
./io/iosectionkeyfile.o \
./io/ioserial.o 

CPP_DEPS += \
./io/convert.d \
./io/ioconfigfile.d \
./io/iodatafile.d \
./io/iodirectory.d \
./io/iofile.d \
./io/iooption.d \
./io/iosectionkeyfile.d \
./io/ioserial.d 


# Each subdirectory must supply rules for building sources it contributes
io/%.o: ../io/%.cpp io/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


