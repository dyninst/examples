#include "CFG.h"
#include "CodeObject.h"
#include "PatchCFG.h"

#include <iostream>
#include <vector>

namespace dp = Dyninst::ParseAPI;
namespace pa = Dyninst::PatchAPI;

/*
 * Traverse the CFG using PatchAPI
 */

int main(int argc, char* argv[]) {
  if(argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " file\n";
    return -1;
  }

  auto* sts = new dp::SymtabCodeSource(argv[1]);
  auto* co = new dp::CodeObject(sts);

  constexpr Dyninst::Address base_address{0UL};
  auto* obj = pa::PatchObject::create(co, base_address);

  std::vector<pa::PatchFunction*> all_funcs;
  obj->funcs(back_inserter(all_funcs));

  for(auto* f : all_funcs) {
    std::cout << "Function: " << f->name() << '\n';

    for(auto* b : f->blocks()) {
      std::cout << "    Block :" << b->format() << '\n';
    }

    std::cout << '\n';
  }
}
