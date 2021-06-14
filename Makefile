
#
# A GNU Makefile
#

InterestingProgram: 
	g++ -g -Wall -o InterestingProgram InterestingProgram.c

clean: 
	rm -f InterestingProgram
