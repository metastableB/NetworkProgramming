# MakeFile for CHORD
CC=g++
CFLAGS=-std=c++11 -Wall -pthread
ifeq ($(debug), 1)   
	CFLAGS += -g -D__DEBUG__ 
endif

# Define all source definitions here
FILES=./main.cpp \
	 ./chord_node.cpp

# Source Targets 
main :
	$(CC) $(CFLAGS) $(FILES) -o chord

clean :
	rm -vf  ./*.out   ./chord   ./*.o   ./*.h.gch  ./*.gch


