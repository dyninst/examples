#
# A GNU Makefile
#
# This Makefile builds the code_patching Dyninst examples
#

# Make sure to set the DYNINST_ROOT environment variable to the directory where
# Dyninst is installed. The directory should contain the include and
# $(PLATFORM)/lib directories
CC = g++
CFLAGS = -g -Wall
EXECNAME = proccounting

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif


$(EXECNAME): $(EXECNAME).o
	$(CC) $(EXECNAME).o $(CFLAGS) -L$(DYNINST_ROOT)/lib  -ldyninstAPI -lsymtabAPI -lcommon -lboost_filesystem -lboost_system -o $(EXECNAME)


$(EXECNAME).o: ./$(EXECNAME).c
	$(CC) $(CFLAGS) -I$(DYNINST_ROOT)/include -c $(EXECNAME).c

clean:
	rm -f $(EXECNAME).o $(EXECNAME)
