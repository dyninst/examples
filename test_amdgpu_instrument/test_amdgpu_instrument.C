
/*
 *  A simple code coverage tool using DyninstAPI
 *
 *  This tool uses DyninstAPI to instrument the functions and basic blocks in
 *  an executable and its shared libraries in order to record code coverage
 *  data when the executable is run. This code coverage data is output when the
 *  rewritten executable finishes running.
 *
 *  The intent of this tool is to demonstrate some capabilities of DyninstAPI;
 *  it should serve as a good stepping stone to building a more feature-rich
 *  code coverage tool on top of Dyninst.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Command line parsing
#include <getopt.h>

// DyninstAPI includes
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_flowGraph.h"
#include "BPatch_function.h"
#include "BPatch_module.h"
#include "BPatch_object.h"
#include "BPatch_point.h"

using namespace Dyninst;

static const char* USAGE = " [-bpsa] <binary> <output binary>\n \
                            -b: Basic block level code coverage\n \
                            -p: Print all functions (including functions that are never executed)\n \
                            -s: Instrument shared libraries also\n \
                            -a: Sort results alphabetically by function name\n";

static const char* OPT_STR = "bpsa";

// configuration options
char const* inBinary = NULL;
char const* outBinary = NULL;
bool includeSharedLib = false;
int printAll = 0;
bool bbCoverage = false;
int alphabetical = 0;

set<string> skipLibraries;

/* Every Dyninst mutator needs to declare one instance of BPatch */
BPatch bpatch;

void initSkipLibraries() {
  /* List of shared libraries to skip instrumenting */
  /* Do not instrument the instrumentation library */
  skipLibraries.insert("libInst.so");
  skipLibraries.insert("libc.so.6");
  skipLibraries.insert("libc.so.7");
  skipLibraries.insert("ld-2.5.so");
  skipLibraries.insert("ld-linux.so.2");
  skipLibraries.insert("ld-lsb.so.3");
  skipLibraries.insert("ld-linux-x86-64.so.2");
  skipLibraries.insert("ld-lsb-x86-64.so");
  skipLibraries.insert("ld-elf.so.1");
  skipLibraries.insert("ld-elf32.so.1");
  skipLibraries.insert("libstdc++.so.6");
  return;
}

bool parseArgs(int argc, char* argv[]) {
  int c;
  while((c = getopt(argc, argv, OPT_STR)) != -1) {
    switch((char)c) {
      case 'b': bbCoverage = true; break;
      case 'p': printAll = 1; break;
      case 's':
        /* if includeSharedLib is set,
         * all libraries linked to the binary will also be instrumented */
        includeSharedLib = true;
        break;
      case 'a': alphabetical = 1; break;
      default: cerr << "Usage: " << argv[0] << USAGE; return false;
    }
  }

  int endArgs = optind;

  if(endArgs >= argc) {
    cerr << "Input binary not specified." << endl << "Usage: " << argv[0] << USAGE;
    return false;
  }
  /* Input Binary */
  inBinary = argv[endArgs];

  endArgs++;
  if(endArgs >= argc) {
    cerr << "Output binary not specified." << endl << "Usage: " << argv[0] << USAGE;
    return false;
  }

  /* Rewritten Binary */
  outBinary = argv[endArgs];

  return true;
}

BPatch_function* findFuncByName(BPatch_image* appImage, char const* funcName) {
  /* fundFunctions returns a list of all functions with the name 'funcName' in
   * the binary */
  BPatch_Vector<BPatch_function*> funcs;
  if(NULL == appImage->findFunction(funcName, funcs) || !funcs.size() || NULL == funcs[0]) {
    cerr << "Failed to find " << funcName << " function in the instrumentation library" << endl;
    return NULL;
  }
  return funcs[0];
}

bool insertFuncEntry(BPatch_binaryEdit* appBin, BPatch_function* curFunc, char const* funcName,
                     BPatch_function* targetFunc, int ) {
  /* Find the instrumentation points */
  vector<BPatch_point*>* funcEntry = curFunc->findPoint(BPatch_entry);
  if(NULL == funcEntry) {
    cerr << "Failed to find entry for function " << funcName << endl;
    return false;
  }

  cout << "Inserting instrumention at function entry of " << funcName << endl;
  /* Create a vector of arguments to the function
   * incCoverage function takes the function name as argument */
  BPatch_Vector<BPatch_snippet*> instArgs;
  //BPatch_constExpr id(funcId);
  //instArgs.push_back(&id);
  BPatch_funcCallExpr instIncExpr(*targetFunc, instArgs);

  /* Insert the snippet at function entry */
  BPatchSnippetHandle* handle =
      appBin->insertSnippet(instIncExpr, *funcEntry, BPatch_callBefore, BPatch_lastSnippet);
  if(!handle) {
    cerr << "Failed to insert instrumention at function entry of " << funcName << endl;
    return false;
  }
  return true;
}


int main(int argc, char* argv[]) {
  if(!parseArgs(argc, argv))
    return EXIT_FAILURE;

  /* Initialize list of libraries that should not be instrumented - relevant
   * only if includeSharedLib is true */
  initSkipLibraries();

  /* Open the specified binary for binary rewriting.
   * When the second parameter is set to true, all the library dependencies
   * as well as the binary are opened */

  BPatch_binaryEdit* appBin = bpatch.openBinary(inBinary, true);
  if(appBin == NULL) {
    cerr << "Failed to open binary" << endl;
    return EXIT_FAILURE;
  }

  /* Open the instrumentation library.
   * loadLibrary loads the instrumentation library into the binary image and
   * adds it as a new dynamic dependency in the rewritten library */
  const char* instLibrary = "funptr1_gfx908.hsaco";
  if(!appBin->loadLibrary(instLibrary)) {
    cerr << "Failed to open instrumentation library" << endl;
    return EXIT_FAILURE;
  }

  BPatch_image* appImage = appBin->getImage();
  /* Find code coverage functions in the instrumentation library */
  BPatch_function* instIncFunc = findFuncByName(appImage, "funptr1");
  
  if(!instIncFunc) {
    return EXIT_FAILURE;
  }


  int funcIndex = 0;

  std::string target("_Z15vectoradd_floatPfPKfS1_ii");
  /* Locate _init */
  BPatch_Vector<BPatch_function*> funcs;
  appImage->findFunction(target.c_str(), funcs);
  if(funcs.size()) {
    printf("funcs.size = %lu\n",funcs.size());
    char funcName[1024];
    BPatch_function* curFunc = funcs[0];
    
    curFunc->getName(funcName, 1024);
    char moduleName[1024];
    BPatch_module * myModule = curFunc->getModule();
    myModule->getFullName(moduleName,1024);

    cerr << "Found function with name " << target << " in module " << moduleName << endl;
    insertFuncEntry(appBin, curFunc, funcName, instIncFunc, funcIndex);
  }else{
    cerr << "Cannot find function with name " << target << endl;
  }

  // Output the instrumented binary
  if(!appBin->writeFile(outBinary)) {
    cerr << "Failed to write output file: " << outBinary << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
