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
//static char *outbinary = "output";
static const char **args = NULL;

bool parseArgs (int argc, char *argv[])
{
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
        if (!parseArgs (argc, argv))
        	return EXIT_FAILURE;

	pathname = argv[1];




	//identifies the application process to be modified
	//BPatch_process *appProc = bpatch.processCreate(pathname, args);
	//BPatch_binaryEdit *appBin = bpatch.openBinary(pathname, true);
	BPatch_process *appProc = bpatch.processCreate(pathname, args);
	BPatch_addressSpace *app = appProc;
	BPatch_image *appImage = app->getImage();
	//std::vector<BPatch_function *> functions;
	//appThread->bpatch.createProcess(pathname, args); 
	//BPatch_image *appImage = app->getImage(); 




	//insert the call counter at the start of interesting prcedure
	vector<BPatch_function*> functions;
	appImage->findFunction("InterestingProcedure", functions);
	//vector < BPatch_function * >::iterator funcIter = functions.begin();
	std::vector<BPatch_point *> *points;
	points = functions[0]->findPoint(BPatch_locEntry);
	//BPatch_function * headFunc = *funcIter;
	//std::vector<BPatch_point*>* points = 
	//	headFunc->findPoint(BPatch_locEntry);

	BPatch_variableExpr *intCounter =
		app->malloc(*appImage->findType("int"));

	BPatch_arithExpr addOne(BPatch_assign, 
			*intCounter, BPatch_arithExpr(BPatch_plus, *intCounter, BPatch_constExpr(1)));
	
	if (!app->insertSnippet(addOne, *points, BPatch_callBefore, BPatch_lastSnippet)) {
		fprintf(stderr, "insertSnippet failed\n");
	        return false;
	}



	//insert the counter printer at the end of main function
	vector<BPatch_function*> func2;
        appImage->findFunction("main", func2);
	std::vector<BPatch_point *> *exitPoint;
	exitPoint = func2[0]->findPoint(BPatch_exit);

	if (!exitPoint || exitPoint->size() == 0) {
        	fprintf(stderr, "No exit points for main\n");
        	exit(1);
    	}

   	 std::vector<BPatch_snippet*> printfArgs;
	BPatch_snippet* fmt =
        new BPatch_constExpr("InterestingProcedure called %d times\n");
       	printfArgs.push_back(fmt);
	BPatch_variableExpr* var = appImage->findVariable("intCounter");
	printfArgs.push_back(var);

	std::vector<BPatch_function*> printfFuncs;
	appImage->findFunction("printf", printfFuncs);


	BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);
	//BPatch_breakPointExpr stop();
	//vector<BPatch_function*> functions2;
	//appImage->findFunction("main", functions2);
	//std::vector<BPatch_point *> *points2;
	//points2 = functions2[0]->findPoint(BPatch_locEntry);
	//app->insertSnippet(BPatch_breakPointExpr(),  *points2, BPatch_callBefore, BPatch_lastSnippet);
	
	
	
	
	// wait for the program to finish
	appProc->continueExecution();
	while (!appProc->isTerminated()) {
		bpatch.waitForStatusChange();
	}

	return EXIT_SUCCESS;
}
