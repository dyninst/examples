#include "Event.h"
#include "PCProcess.h"

#include <iostream>
#include <string>

namespace pc = Dyninst::ProcControlAPI;

pc::Process::cb_ret_t on_thread_create(pc::Event::const_ptr ev) {
  // Callback when the target process creates a thread.
  auto new_thrd = ev->getEventNewThread()->getNewThread();

  std::cout << "Got a new thread with LWP " << new_thrd->getLWP() << '\n';
  return pc::Process::cbDefault;
}

int main(int argc, char* argv[]) {

  std::vector<std::string> args(argv + 1, argv + argc);
  pc::Process::ptr proc = pc::Process::createProcess(argv[0], args);

  // Tell ProcControlAPI about our callback function
  pc::Process::registerEventCallback(pc::EventType::ThreadCreate, on_thread_create);

  // Run the process and wait for it to terminate.
  proc->continueProc();
  while(!proc->isTerminated())
    pc::Process::handleEvents(true);
}
