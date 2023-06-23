# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -std=c++11

INC=-Ilibs/csv_ingestion/include -Ilibs/lla/include
#$(OBJS) = $(patsubst %.o,obj/%.o,file0.o file1.o)
 
# The build target 
TARGET = src/main
EXECUTABLE = bin/app
OBJ_DIR = obj/
CSV_ING_PATH = libs/csv_ingestion/
LLA_PATH = libs/lla/

all: clean csv_ingestion lla executable


executable:
	g++ ${CFLAGS}  ${INC} -o ${EXECUTABLE} ${TARGET}.cpp obj/csv_ingestion.o obj/lla.o
 
#INC=-I/usr/informix/incl/c++ -I/opt/informix/incl/public

csv_ingestion:
	g++ ${CFLAGS} ${INC} -c -o ${OBJ_DIR}csv_ingestion.o ${CSV_ING_PATH}src/csv_ingestion.cpp 

lla:
	g++ ${CFLAGS} ${INC} -c -o ${OBJ_DIR}lla.o ${LLA_PATH}src/lla.cpp 

clean:
	$(RM) $(EXECUTABLE)
	$(RM) $(OBJ_DIR)*