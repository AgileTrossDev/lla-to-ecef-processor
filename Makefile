# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -std=c++11

INC=-Ilibs/csv_ingestion/include -Ilibs/coordinates/include
#$(OBJS) = $(patsubst %.o,obj/%.o,file0.o file1.o)
 
# The build target 
TARGET = src/main
EXECUTABLE = bin/app
OBJ_DIR = obj/
CSV_ING_PATH = libs/csv_ingestion/
COORDINATES_PATH = libs/coordinates/

all: clean csv_ingestion coordinates executable


executable:
	g++ ${CFLAGS}  ${INC} -o ${EXECUTABLE} ${TARGET}.cpp obj/csv_ingestion.o obj/lla.o obj/ecef.o
 
#INC=-I/usr/informix/incl/c++ -I/opt/informix/incl/public

csv_ingestion:
	g++ ${CFLAGS} ${INC} -c -o ${OBJ_DIR}csv_ingestion.o ${CSV_ING_PATH}src/csv_ingestion.cpp 

coordinates:
	g++ ${CFLAGS} ${INC} -c -o ${OBJ_DIR}lla.o ${COORDINATES_PATH}src/lla.cpp 
	g++ ${CFLAGS} ${INC} -c -o ${OBJ_DIR}ecef.o  ${COORDINATES_PATH}src/ecef.cpp

clean:
	$(RM) $(EXECUTABLE)
	$(RM) $(OBJ_DIR)*