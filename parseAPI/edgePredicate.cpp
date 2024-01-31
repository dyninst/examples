#include "CFG.h"

#include <vector>

namespace dp = Dyninst::ParseAPI;

void edge(dp::Function* func) {
  std::vector<dp::Block*> work;
  work.push_back(func->entry());

  dp::Intraproc intra_pred; // ignore calls, returns
  dp::Interproc inter_pred;

  while(!work.empty()) {
    dp::Block* block = work.back();
    work.pop_back();

    dp::Block::edgelist const& targets = block->targets();

    for(dp::Edge* e : targets) {
      if(intra_pred.pred_impl(e)) {
        // Examine the Intraprocedural edge
      }
      if(inter_pred.pred_impl(e)) {
        // Examine the Interprocedural edge
      }
    }
  }
}
