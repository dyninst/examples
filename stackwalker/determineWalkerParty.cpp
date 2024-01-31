#include "procstate.h"
#include "walker.h"

// How to determine if first-party or third-party walker

namespace sw = Dyninst::Stackwalker;

int main() {
  auto* walker = sw::Walker::newWalker();
  auto* debugger = dynamic_cast<sw::ProcDebug*>(walker->getProcessState());

  if(debugger) {
    // 3rd party walker
  } else {
    // 1st party walker
  }
}
