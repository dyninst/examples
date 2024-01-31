#include "Absloc.h"
#include "CFG.h"
#include "stackanalysis.h"

#include <cstdio>
#include <utility>
#include <vector>

namespace pa = Dyninst::ParseAPI;

void StackHeight(pa::Function* func, pa::Block* block) {
  // Get the address of the first instruction of the block
  Dyninst::Address addr = block->start();

  // Get the stack heights at that address
  Dyninst::StackAnalysis sa(func);
  std::vector<std::pair<Dyninst::Absloc, Dyninst::StackAnalysis::Height>> heights;
  sa.findDefinedHeights(block, addr, heights);

  // Print out the stack heights
  for(auto h : heights) {
    const Dyninst::Absloc& loc = h.first;
    const Dyninst::StackAnalysis::Height& height = h.second;
    printf("%s := %s\n", loc.format().c_str(), height.format().c_str());
  }
}
