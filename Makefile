CC = g++

CFLAGS = -Wall `root-config --cflags`

CLIBS = `root-config --glibs` -lboost_system -lboost_filesystem

all: BT

BT:
	$(CC) betterHadd.cpp -o bhadd.exe $(CLIBS) $(CFLAGS) -std=c++11
			
clean: 
	rm -rf *o 
