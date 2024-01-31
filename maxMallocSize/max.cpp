#include "BPatch.h"
#include "BPatch_function.h"
#include "BPatch_point.h"

#include <stdio.h>

using namespace std;
using namespace Dyninst;

BPatch bpatch;
BPatch_variableExpr* maxarg = NULL;

/* This function is registered as a BPatchExitCallback,
 * meaning that it will be invoked immediately prior to
 * the mutatee's exit.
 *
 * It reads the value of the global maxarg variable and
 * prints it to stdout
 */
static void readMaxArg(BPatch_thread*, BPatch_exitType) {
  int maximum_malloc_argument;
  maxarg->readValue(&maximum_malloc_argument);
  printf("\nThe largest memory request was: malloc( %d )\n", maximum_malloc_argument);
}

/* int main(int argc, char *argv[])
 *
 * 1. Process control:
 * 1a. Check command line arguments
 * 1b. Create process, get BPatch_image object
 *
 * 2. Register ExitCallback function, readMaxArg
 *
 * 3. Initialization instrumentation:
 * 3a. Create maxarg variable
 * 3b. Form initialization snippet
 * 3c. Find main through the BPatch_image class
 * 3d. Find the entry point of main
 * 3e. Insert snippet at entry point of main
 *
 * 4. Malloc Instrumentation: maxarg = max(maxarg,curarg)
 * 4a. Form conditional snippet
 * 4b. Find the libc module
 * 4c. Find malloc in the libc module
 * 4d. Insert snippet at entry point of malloc
 *
 * 5. More Process Control
 * 5a. Continue the mutatee, which is paused at the entry point of main
 * 5b. Continue process through status change events until termination
 */
int main(int argc, char* argv[]) {

  /*  1. Process control....................................................*/

  /*  1a. Check command line arguments                                      */
  if(argc < 2) {
    fprintf(stderr, "Usage: %s prog_filename [args]\n", argv[0]);
    return 1;
  }

  /*  1b. Create process, get BPatch_image object                           */
  BPatch_process* proc = bpatch.processCreate(argv[1], (const char**)(argv + 1));
  assert(proc);
  BPatch_image* image = proc->getImage();

  /*  2. Register ExitCallback function,
        static void readMaxArg(BPatch_thread* , BPatch_exitType)............*/
  bpatch.registerExitCallback(readMaxArg);

  /*  3. Initialization instrumentation.....................................*/

  /*  3a. Create max_size variable                                          */
  // maxarg is declared globally so that the callback function can access it
  maxarg = proc->malloc(*image->findType("int"));

  /*  3b. Form initialization snippet                                       */
  BPatch_constExpr zero(0);
  BPatch_arithExpr initSnippet(BPatch_assign, *maxarg, zero);

  /*  3c. Find main through the BPatch_image class                          */
  BPatch_Vector<BPatch_function*> mainFuncs;
  image->findFunction("main", mainFuncs);
  assert(mainFuncs.size() != 0);
  BPatch_function* main = mainFuncs[0];

  /*  3d. Find the entry point of main                                      */
  std::vector<BPatch_point*>* mainEntryPoints = main->findPoint(BPatch_entry);
  BPatch_point* mainEntry = (*mainEntryPoints)[0];

  /*  3e. Insert snippet at entry point of main                             */
  proc->insertSnippet(initSnippet, *mainEntry, BPatch_firstSnippet);

  /*  4. Malloc Instrumentation: maxarg = max(maxarg,curarg)................*/

  /*  4a. Form conditional snippet                                          */
  BPatch_paramExpr curarg(0);
  BPatch_boolExpr compare_cur_max(BPatch_gt, curarg, *maxarg);
  BPatch_arithExpr set_max(BPatch_assign, *maxarg, curarg);
  BPatch_ifExpr conditionalAssignment(compare_cur_max, set_max);

  /*  4b. Find the libc module                                              */
  BPatch_module* libc = image->findModule("libc", true);
  assert(libc);

  /*  4c. Find malloc in the libc module                                    */
  BPatch_Vector<BPatch_function*> mallocFuncs;
  libc->findFunction("malloc", mallocFuncs);
  assert(mallocFuncs.size());
  BPatch_function* bp_malloc = mallocFuncs[0];

  /*  4d. Find the entry point of malloc                                    */
  std::vector<BPatch_point*>* mallocEntryPoints = bp_malloc->findPoint(BPatch_entry);
  BPatch_point* mallocEntry = (*mallocEntryPoints)[0];

  /*  4d. Insert snippet at entry point of malloc                           */
  proc->insertSnippet(conditionalAssignment, *mallocEntry, BPatch_firstSnippet);

  /*  5. More Process Control...............................................*/

  /*  5a. Continue the mutatee, which is paused at the entry point of main  */
  proc->continueExecution();

  /*  5b. Continue process through status change events until termination   */
  while(!proc->isTerminated())
    bpatch.waitForStatusChange();
  return 0;
}
