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
#include "BPatch_binaryEdit.h"
#include "BPatch_function.h"
#include "BPatch_point.h"

using namespace Dyninst;

//instance to access BPatch lib
BPatch bpatch; 


// configuration options
static char *pathname = NULL;
static char *outbinary = "output";
static const char **args = NULL;

bool parseArgs (int argc, char *argv[])
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

	// Output the instrumented binary
    appProc->continueExecution();
while (!appProc->isTerminated()) {
	bpatch.waitForStatusChange();
}

	return EXIT_SUCCESS;
}
