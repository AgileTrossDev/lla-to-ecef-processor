# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall
 
# The build target 
TARGET = src/main
EXECUTABLE = bin/app

all: 
	g++ -o ${EXECUTABLE} ${TARGET}.cpp
#	$(TARGET): $(TARGET).cpp 
#	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(TARGET).cpp
 
clean:
	$(RM) $(TARGET)