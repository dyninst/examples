#
# A GNU Makefile
#

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif

patchmgr_addNoopSnippet:	patchmgr_addNoopSnippet.o
	g++ patchmgr_addNoopSnippet.o -g -L$(DYNINST_ROOT)/lib -ldyninstAPI -linstructionAPI -lpatchAPI -lcommon -lboost_filesystem -lboost_system -o patchmgr_addNoopSnippet
	
patchmgr_addNoopSnippet.o:
	g++ -g -Wall -I$(DYNINST_ROOT)/include -c patchmgr_addNoopSnippet.c

clean:
	rm -f patchmgr_addNoopSnippet.o patchmgr_addNoopSnippet
