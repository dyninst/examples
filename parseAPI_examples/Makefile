#
# A GNU Makefile
#

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif


all: loopAnalysis funcDisssemb traverseCFG

loopAnalysis: loopAnalysis.o
	 g++ loopAnalysis.o   -g -L$(DYNINST_ROOT)/lib \
	       -ldyninstAPI -lsymtabAPI -lparseAPI -linstructionAPI -lcommon \
	       -lboost_filesystem -lboost_system -o  loopAnalysis

loopAnalysis.o: g++ -g -Wall -I$(DYNINST_ROOT)/include -c loopAnalysis.c

funcDisssemb: funcDissemb.o
	g++ -std=c++0x funcDisssemb.o -o funcDisssemb -L$(DYNINST_ROOT)/lib \
	      -L~/private/dyninst/dwarf -L/p/paradyn/packages/libelf/lib \
	      -L/p/paradyn/packages/libdwarf/lib \
	      -lparseAPI  -linstructionAPI -lsymtabAPI  -lcommon -lboost_filesystem -lboost_system

funcDisssemb.o: g++ -g -Wall -I$(DYNINST_ROOT)/include -c funcDisssemb.c

traverseCFG: traverseCFG.o
	g++ -std=c++0x traverseCFG.o  -g -L$(DYNINST_ROOT)/lib \
	    -ldyninstAPI -lsymtabAPI -lparseAPI -lcommon \
	    -lboost_filesystem -lboost_system -o traverseCFG

traverseCFG.o: g++ -g -Wall -I$DYNINST_ROOT/include -c traverseCFG.c

clean: rm -f *.o loopAnalysis funcDisssemb traverseCFG 


