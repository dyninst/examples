#include "AbslocInterface.h"
#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_basicBlock.h"
#include "BPatch_flowGraph.h"
#include "BPatch_function.h"
#include "BPatch_thread.h"
#include "CFG.h"
#include "InstructionDecoder.h"
#include "SymEval.h"

#include <iostream>
#include <vector>

namespace ia = Dyninst::InstructionAPI;
namespace df = Dyninst::DataflowAPI;
namespace pa = Dyninst::ParseAPI;

// Symbolic evaluation of all instructions in a function in an x86 binary

int main(int argc, char* argv[]) {
  if(argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <test_program> <func_to_analyze>\n";
    return -1;
  }

  BPatch bpatch;
  BPatch_addressSpace* app = bpatch.openBinary(argv[1], true);
  BPatch_image* appImage = app->getImage();

  auto* function = [argv, &appImage]() {
    BPatch_Vector<BPatch_function*> function;
    appImage->findFunction(argv[2], function);
    return function[0];
  }();

  BPatch_flowGraph* cfg = function->getCFG();
  BPatch_Set<BPatch_basicBlock*> blocks;
  cfg->getAllBasicBlocks(blocks);

  Dyninst::AssignmentConverter converter(true, false);
  pa::Function* pa_function = pa::convert(function);

  for(BPatch_basicBlock* block : blocks) {
    std::vector<std::pair<ia::Instruction, Dyninst::Address>> insns;
    block->getInstructions(insns);

    pa::Block* pa_block = pa::convert(block);

    for(auto ins : insns) {
      std::vector<Dyninst::Assignment::Ptr> assigns;
      std::cout << "Found instruction " << ins.first.format() << '\n';

      converter.convert(ins.first, ins.second, pa_function, pa_block, assigns);

      // Build a map stating which assignments we're interested in
      for(auto p : assigns) {
        // Symbolically evaluate
        std::pair<Dyninst::AST::Ptr, bool> sym_ins = df::SymEval::expand(p);
        if(!sym_ins.second) {
          std::cerr << "Failed to expand\n";
          continue;
        }
        std::cout << "-----------------\n";
        std::cout << sym_ins.first->format();
        std::cout << "\n====================\n";
      }
    }
  }
}
