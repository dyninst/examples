CC	= /usr/bin/g++
CFLAGS	= -Wall
target= retee

all:$(target)

$(target): $(target).o
	$(CC) $(target).o -L$(DYNINST_ROOT)/lib \
	    -ldyninstAPI -lsymtabAPI -lcommon -lboost_filesystem -lboost_system -ltbbmalloc -linstructionAPI -o $(target)

$(target).o: ./$(target).cpp
	$(CC) -g -c $(CFLAGS) -I$(DYNINST_ROOT)/include $(target).cpp


clean:
	@rm -f $(target).o $(target) core

