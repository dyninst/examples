#include "frame.h"
#include "walker.h"

#include <iostream>
#include <string>
#include <vector>

// Walk and print the call stack of the currently running thread

namespace sw = Dyninst::Stackwalker;

int main() {
  std::vector<sw::Frame> frames;
  auto* walker = sw::Walker::newWalker();
  walker->walkStack(frames);

  std::string name;
  for(auto const& f : frames) {
    f.getName(name);
    std::cout << "Found function " << name << '\n';
  }
}
