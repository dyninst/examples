#include "BPatch.h"
#include "BPatch_function.h"
#include "BPatch_process.h"
#include "BPatch_snippet.h"
#include "dynC.h"

#include <fstream>
#include <iostream>
#include <string>

#define STATEMENT_PATH "testStatements"
#define STATEMENT_PATH_2 "testStatements2"

const char* MUTATEE_PATH = "dync_mutatee";
const char* MUTATEE_ARGS[3];
const char* MODULE_NAME = "mutatee.cpp";

BPatch bpatch;

int main() {

  std::ifstream myfile(STATEMENT_PATH);
  if(!myfile) {
    std::cerr << "Unable to open '" << STATEMENT_PATH << "'\n";
    return -1;
  }

  std::ifstream myfile2(STATEMENT_PATH_2);
  if(!myfile2) {
    std::cerr << "Unable to open '" << STATEMENT_PATH_2 << "'\n";
    return -1;
  }

  FILE* myCFILE = fopen(STATEMENT_PATH, "r");

  BPatch_addressSpace* appProc;
  bool rewrite = false;

  if(rewrite) {
    appProc = bpatch.openBinary(MUTATEE_PATH, true);
  } else {
    appProc = bpatch.processCreate(MUTATEE_PATH, MUTATEE_ARGS);
  }
  if(!appProc) {
    std::cerr << "Unable to create an address space\n";
    return -1;
  }

  BPatch_image* appImage = appProc->getImage();
  BPatch_module* mutatee = appImage->findModule(MODULE_NAME);

  if(!mutatee) {
    std::cerr << "Bad Mutatee!\n";
    return -1;
  }

  appProc->malloc(*appImage->findType("long"), std::string("globalVar"));

  const std::vector<BPatch_function*>* functions = mutatee->getProcedures();
  std::vector<BPatch_point*>* entry_points = (*functions)[0]->findPoint(BPatch_entry);
  std::vector<BPatch_point*>* exit_points = (*functions)[0]->findPoint(BPatch_exit);

  for(auto* f : *mutatee->getProcedures()) {
    entry_points->push_back((*f->findPoint(BPatch_entry))[0]);
    exit_points->push_back((*f->findPoint(BPatch_exit))[0]);
  }

  /////////////////////////////////////////////////////////
  std::map<BPatch_point*, BPatch_snippet*>*entry_snippets, exit_snippets;
  entry_snippets = dynC_API::createSnippet(myCFILE, *entry_points);

  if(!entry_snippets) {
    std::cerr << "entry_snippets is null.\n";
    return -1;
  }

  for(auto const& entry : *entry_snippets) {
    if(!entry.first) {
      std::cerr << "Entry point is null.\n";
      return -1;
    }
    if(!entry.second) {
      std::cerr << "Entry snippet is null.\n";
      return -1;
    }
    std::cout << "Snippet inserted\n";
    std::cout << "Point's function is " << entry.first->getFunction()->getName() << "\n";
    BPatchSnippetHandle* handle = appProc->insertSnippet(*entry.second, *entry.first);
    std::cout << "Handle is " << ((!handle) ? "null" : "not null") << "\n";
  }

  if(!rewrite) {
    BPatch_process* aProc = static_cast<BPatch_process*>(appProc);
    aProc->continueExecution();

    while(!aProc->isTerminated()) {
      bpatch.waitForStatusChange();
    }

    if(aProc->terminationStatus() == ExitedNormally) {
      printf("Application exited with code %d\n", aProc->getExitCode());
    } else if(aProc->terminationStatus() == ExitedViaSignal) {
      printf("!!! Application exited with signal %d\n", aProc->getExitSignal());
    } else {
      printf("Unknown application exit\n");
    }
  } else {
    BPatch_binaryEdit* aProc = static_cast<BPatch_binaryEdit*>(appProc);
    aProc->writeFile("myMutatee.out");
  }
}
