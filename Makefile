# makefile for files project

CCC = g++
CXXFLAGS = -g -Wall -pthread 
CXXLINK = $(CCC)
# LIBS
OBJS = main.o Account.o Bank.o ATM.o Log.o


# creating executable

files: $(OBJS) 
	$(CXXLINK) $(CXXFLAGS) -o Bank $(OBJS) 
	
# creating o-files

Bank.o: Bank.cpp Bank.h ATM.h Account.h Log.h
ATM.o: ATM.cpp ATM.h Bank.h Account.h Log.h
Account.o: Account.cpp Account.h
Log.o: Log.cpp Log.h
main.o: main.cpp Bank.h ATM.h Log.h

clean:
	rm -f *.o *.bak *~"#"* core
