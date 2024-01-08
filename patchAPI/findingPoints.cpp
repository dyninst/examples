#include "AddrSpace.h"
#include "CFG.h"
#include "CodeObject.h"
#include "PatchCFG.h"
#include "PatchMgr.h"
#include "PatchObject.h"

#include <iostream>
#include <vector>

namespace dp = Dyninst::ParseAPI;
namespace pa = Dyninst::PatchAPI;

/*
 * Find entry points for every function
 */

int main(int argc, char* argv[]) {
  if(argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " file\n";
    return -1;
  }

  auto* sts = new dp::SymtabCodeSource(argv[1]);
  auto* co = new dp::CodeObject(sts);

  constexpr Dyninst::Address base_address{0UL};
  auto* patch_object = pa::PatchObject::create(co, base_address);

  auto* address_space = pa::AddrSpace::create(patch_object);
  auto patch_manager = pa::PatchMgr::create(address_space);

  std::vector<pa::PatchFunction*> functions;
  patch_object->funcs(std::back_inserter(functions));

  for(auto* f : functions) {
    std::vector<pa::Point*> entryPoints;
    patch_manager->findPoints(pa::Scope(f), pa::Point::FuncEntry, std::back_inserter(entryPoints));

    std::cout << f->name() << " has " << entryPoints.size() << " entry point(s)\n";
  }
}
