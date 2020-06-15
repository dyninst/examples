/*
 * A  simple debugger
 * that allows a user to set conditional breakpoints at any locations 
 * that can be instrumented by the Dyninst API.
 * The user can add or remove any number of breakpoints 
 * during the running of the application.
 */

#include <stdlib.h>
#include <stdio.h>

// DyninstAPI includes
#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_process.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_function.h"
#include "BPatch_point.h"

using namespace std;
using namespace Dyninst;

//instance to access BPatch lib
BPatch bpatch;



/*
 *
 */
int main (int argc, char *argv[])
{
}
