#
# A GNU Makefile
#

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif


all: loopAnalysis funcDisassemb traverseCFG

loopAnalysis: loopAnalysis.o
	 g++ loopAnalysis.o   -g -L$(DYNINST_ROOT)/lib \
	       -ldyninstAPI -lsymtabAPI -lparseAPI -linstructionAPI -lcommon \
	       -lboost_filesystem -lboost_system -o  loopAnalysis

loopAnalysis.o: 
	g++ -g -Wall -I$(DYNINST_ROOT)/include -c loopAnalysis.c

funcDisassemb: funcDisassemb.o
	g++ -std=c++0x funcDisassemb.o -o funcDisassemb -L$(DYNINST_ROOT)/lib \
	      -L~/private/dyninst/dwarf -L/p/paradyn/packages/libelf/lib \
	      -L/p/paradyn/packages/libdwarf/lib \
	      -lparseAPI  -linstructionAPI -lsymtabAPI  -lcommon -lboost_filesystem -lboost_system

funcDisassemb.o: 
	g++ -g -Wall -I$(DYNINST_ROOT)/include -c funcDisassemb.c

traverseCFG: traverseCFG.o
	g++ -std=c++0x traverseCFG.o  -g -L$(DYNINST_ROOT)/lib \
	    -ldyninstAPI -lsymtabAPI -lparseAPI -lcommon \
	    -lboost_filesystem -lboost_system -o traverseCFG

traverseCFG.o: 
	g++ -g -Wall -I$(DYNINST_ROOT)/include -c traverseCFG.c

clean: 
	rm -f *.o loopAnalysis funcDisassemb traverseCFG 


