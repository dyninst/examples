#include <stdio.h>
#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "AddrSpace.h"
#include "CodeObject.h"
#include "PatchMgr.h"
#include "PatchObject.h"
//#include "PatchFunction.h"
#include "PatchCFG.h"
#include "Point.h"
#include "PatchCallback.h"
//#include "CFG.h"

using namespace std;
using namespace Dyninst;
using namespace PatchAPI;
using namespace ParseAPI;
using Dyninst::PatchAPI::AddrSpace;

using Dyninst::ParseAPI::CodeSource;
using Dyninst::InstructionAPI::InstructionDecoder;
using Dyninst::InstructionAPI::Instruction;
using Dyninst::PatchAPI::PatchMgr;
using Dyninst::PatchAPI::PatchMgrPtr;
using Dyninst::PatchAPI::PointMaker;
using Dyninst::PatchAPI::InstancePtr;
using Dyninst::PatchAPI::Point;
using Dyninst::PatchAPI::PointSet;
using Dyninst::PatchAPI::PatchBlock;
using Dyninst::PatchAPI::PatchEdge;
using Dyninst::PatchAPI::PatchFunction;

int main (int argc, char* argv[]) {

	SymtabCodeSource *sts;
	Address code_base = NULL;

	sts = new SymtabCodeSource( argv[1] );
	

	ParseAPI::CodeObject* co = new CodeObject( sts );
PatchObject* obj = PatchObject::create(co, code_base);

// Find all functions in the object
std::vector<PatchFunction*> all;
obj->funcs(back_inserter(all));

for (std::vector<PatchFunction*>::iterator fi = all.begin();
     fi != all.end(); fi++) {
  // Print out each function's name
  PatchFunction* func = *fi;
  std::cout << func->name() << std::endl;

  const PatchFunction::Blockset& blks = func->blocks();
  for (PatchFunction::BlockSet::iterator bi = blks.begin();
       bi != blks.end(); bi++) {
    // Print out each block's size
    PatchBlock* blk = *bi;
    std::cout << "\tBlock size:" << blk->size() << std::endl;
  }
}
}
