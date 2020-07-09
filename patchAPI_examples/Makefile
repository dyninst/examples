#
# A GNU Makefile
#

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif

mgr:	mgr.o
	g++ mgr.o -g -L$(DYNINST_ROOT)/lib -ldyninstAPI -linstructionAPI -lpatchAPI -lcommon -lboost_filesystem -lboost_system -o mgr
	
mgr.o:
	g++ -g -Wall -I$(DYNINST_ROOT)/include -c mgr.c

clean:
	rm -f mgr.o mgr
