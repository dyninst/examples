CC	= /usr/bin/g++
CFLAGS	= -Wall
target= instrumenting_a_function
mutatee= InterestingProgram

all:$(target) $(mutatee)

$(target): $(target).o
	$(CC) $(target).o -L$(DYNINST_ROOT)/lib \
	    -ldyninstAPI -lsymtabAPI -lcommon -lboost_filesystem -lboost_system -ltbbmalloc -linstructionAPI -o $(target)

$(target).o: ./$(target).cpp
	$(CC) -g -c $(CFLAGS) -I$(DYNINST_ROOT)/include $(target).cpp

$(mutatee): $(mutatee).o
	$(CC) $(mutatee).o -L$(DYNINST_ROOT)/lib \
	    -ldyninstAPI -lsymtabAPI -lcommon -lboost_filesystem -lboost_system -ltbbmalloc -o $(mutatee)

$(mutatee).o: ./$(mutatee).cpp
	$(CC) -g -c $(CFLAGS) -I$(DYNINST_ROOT)/include $(mutatee).cpp



clean:
	@rm -f $(target).o $(target) core
	@rm -f $(mutatee).o $(mutatee) core

