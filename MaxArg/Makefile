CC	= /usr/bin/g++
CFLAGS	= -Wall
EXECNAME= maxarg
#EXECNAME= retee

$(EXECNAME): $(EXECNAME).o
	$(CC) $(EXECNAME).o -L$(DYNINST_ROOT)/lib \
	    -ldyninstAPI -lsymtabAPI -lcommon -lboost_filesystem -lboost_system -ltbbmalloc -o $(EXECNAME)

$(EXECNAME).o: ./$(EXECNAME).C
	$(CC) -g -c $(CFLAGS) -I$(DYNINST_ROOT)/include $(EXECNAME).C

clean:
	@rm -f $(EXECNAME).o $(EXECNAME) core

