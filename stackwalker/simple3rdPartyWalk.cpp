#include "frame.h"
#include "walker.h"

#include <thread>
#include <vector>

// Collect a stack walk every five seconds.

namespace sw = Dyninst::Stackwalker;

void walk(Dyninst::PID pid) {
  auto* walker = sw::Walker::newWalker(pid);
  std::vector<sw::Frame> swalk;
  for(;;) {
    walker->walkStack(swalk);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);
  }
}
