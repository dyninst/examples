#include "CFG.h"

#include <vector>

namespace dp = Dyninst::ParseAPI;

void GetLoopInFunc(dp::Function* f) {
  std::vector<dp::Loop*> loops;
  f->getLoops(loops);

  for(dp::Loop* loop : loops) {
    // Get all the entry blocks
    std::vector<dp::Block*> entries;
    loop->getLoopEntries(entries);

    // Get all the blocks in the loop
    std::vector<dp::Block*> blocks;
    loop->getLoopBasicBlocks(blocks);

    // Get all the back edges in the loop
    std::vector<dp::Edge*> backEdges;
    loop->getBackEdges(backEdges);
  }
}
