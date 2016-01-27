# MakeFile PDG
# @author: metastableB

# Variable declarations
CC=	g++
CFLAGS=-std=c++11 -Wall
#ifeq ($(debug), 1)   
	#CFLAGS += -g -D_DEBUG_
#endif

# Define all source definitions here
FILES=./source.cpp \
	  ./packet.cpp \
	  ./switch.cpp \
	  ./temporal_object.cpp\
	  ./handler.cpp\
	  ./main.cpp \

# Source Targets 
all :
	$(CC) $(CFLAGS) $(FILES) -o ./sim

clean :
	rm -vf  ./*.out   ./sim   ./*.o   ./*.h.gch  ./*.gch
