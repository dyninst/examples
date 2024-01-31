#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

namespace sc = std::chrono;
using milliseconds = sc::duration<unsigned int, std::milli>;

int dostuff(milliseconds delay) {
  std::this_thread::sleep_for(delay);
  return static_cast<int>(delay.count()) * 2;
}

void doitA(milliseconds delay) { dostuff(delay); }

void doitB(milliseconds delay) {
  dostuff(delay);
  dostuff(delay);
}

int doitC(milliseconds delay) { return dostuff(delay); }

int doitD(milliseconds delay) { return dostuff(delay); }

int main(int argc, char* argv[]) {
  milliseconds delay{800};
  if(argc > 1) {
    int c = atoi(argv[1]);
    if(c < 0) {
      std::cerr << "Cannot have negative delay time\n";
      return -1;
    }
    delay = milliseconds{c};
  }
  doitA(delay);
  doitB(delay);
  return doitC(delay) + doitD(delay);
}
