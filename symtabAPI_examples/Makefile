#
# A GNU Makefile
#

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif

ManipulateObj:
	g++ manipulateObj.c -g -I$(DYNINST_ROOT)/include -L$(DYNINST_ROOT)/lib -ldyninstAPI -lsymtabAPI -lcommon -lboost_filesystem -lboost_system -ltbb -ldyninstAPI -lsymtabAPI -lsymLite -o manipulateObj -Wall

ManipulateObj.o:
	g++ -g -Wall -I$(DYNINST_ROOT)/include -c ManipulateObj.c

clean: rm -f ManipulateObj.o ManipulateObj
