#include "frame.h"
#include "procstate.h"
#include "walker.h"

#include <sys/select.h>
#include <thread>
#include <vector>

// Collect a stack walk every five seconds.

namespace sw = Dyninst::Stackwalker;

void walk(Dyninst::PID pid) {
  auto* walker = sw::Walker::newWalker(pid);
  std::vector<sw::Frame> swalk;

  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  for(;;) {
    walker->walkStack(swalk);
    int max = 1;
    fd_set readfds, writefds, exceptfds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    FD_SET(sw::ProcDebug::getNotificationFD(), &readfds);

    for(;;) {
      int result = select(max, &readfds, &writefds, &exceptfds, &timeout);
      if(FD_ISSET(sw::ProcDebug::getNotificationFD(), &readfds)) {
        // Debug event
        sw::ProcDebug::handleDebugEvent();
      }
      if(result == 0) {
        // Timeout
        break;
      }
    }
  }
}
