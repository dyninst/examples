/*
 * A  short example that inserts instrumentation
 * into a target procedure to count the number of times 
 * the procedure is called. Although a trivial example,
 * it is useful to illustrate the key features of the API. 
 * 
 * Corresponds to example 5.1 in the paper.
 * */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

using namespace std;

// Command line parsing
#include <getopt.h>

// DyninstAPI includes
#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_process.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_function.h"
#include "BPatch_point.h"

using namespace Dyninst;

//instance to access BPatch lib
BPatch bpatch; 


// configuration options
static char *pathname = NULL;
//ProcessCreat() accepts only constant char**
//if the program takes args, change the length equal to the number of args
//static const char *args[n] = NULL;

bool parseArgs (int argc, char *argv[])
{
	if (argc == 1)
	{
		return false;
	}
	else
	{
		pathname = argv[1];
		if ( argc > 2)
		{
			/* uncomment to populate the args
			for ( size_t i = 2; i < len; i++)
			{
				args[i-2] = argv[i];
			}
			*/
		}
	}
	return true;
}

bool instrumentCounter()
{
	return true;
}

bool instrumentPrint()
{
        return true;
}


/*
 *
 */
int main (int argc, char *argv[])
{
        if (!parseArgs (argc, argv)) return EXIT_FAILURE;

	
	//identifies the application process to be modified
	//replace NULL with args in case mutantee requires args
	BPatch_process *appProc = bpatch.processCreate(pathname, NULL);
	BPatch_addressSpace *app = appProc;
	BPatch_image *appImage = app->getImage();


	//insert the call counter snippetat the start of interesting prcedure
	//locate target function InterestingProcedure
	std::vector<BPatch_function*> functions;
	appImage->findFunction("InterestingProcedure", functions);
	//locate the start of the function
	std::vector<BPatch_point *> *points;
	points = functions[0]->findPoint(BPatch_locEntry);
	
	//construct snippet
	//int intCounter = 0
	BPatch_variableExpr *intCounter =
		app->malloc(*appImage->findType("int"), "intCounter");
	//at the start of InterestingProcedure, intCounter++;
	BPatch_arithExpr addOne(BPatch_assign, 
			*intCounter, BPatch_arithExpr(BPatch_plus, *intCounter, BPatch_constExpr(1)));
	//insert counter snippet
	if (!app->insertSnippet(addOne, *points)) {
		fprintf(stderr, "insertSnippet failed\n");
	        return false;
	}


	//insert the counter printer at the end of main function
	//find main function
	std::vector<BPatch_function*> func2;
        appImage->findFunction("main", func2);
	std::vector<BPatch_point *> *exitPoint;
	exitPoint = func2[0]->findPoint(BPatch_exit);

	if (!exitPoint || exitPoint->size() == 0) {
        	fprintf(stderr, "No exit points for main\n");
        	exit(1);
    	}

	//construct printf snippet
   	std::vector<BPatch_snippet*> printfArgs;
	BPatch_snippet* fmt =
        new BPatch_constExpr("InterestingProcedure called %d times\n");
       	printfArgs.push_back(fmt);
	//put intCounter to printf args
	BPatch_variableExpr* var = intCounter;
	printfArgs.push_back(var);
	//find printf
	std::vector<BPatch_function*> printfFuncs;
	appImage->findFunction("printf", printfFuncs);
	BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);
	//insert printf snippet
	app->insertSnippet(printfCall, *exitPoint);	
	
	
	// wait for the program to finish
	appProc->continueExecution();
	while (!appProc->isTerminated()) {
		bpatch.waitForStatusChange();
	}

	return EXIT_SUCCESS;
}
