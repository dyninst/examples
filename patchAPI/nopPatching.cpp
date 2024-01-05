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
 * Insert an x86 nop slide in every function
 */

class NopSlide : public pa::Snippet {
  int count{4};

public:
  NopSlide(int num_nops) : count{num_nops} {}

  bool generate(pa::Point*, Dyninst::Buffer& buffer) override {
    uint8_t byte = 0x90;
    for(int i = 0; i < count; i++) {
      buffer.push_back(byte);
    }
    return true;
  }
};

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

  pa::Patcher patcher(patch_manager);
  auto snippet = pa::Snippet::create(new NopSlide(10));

  std::vector<pa::PatchFunction*> functions;
  patch_object->funcs(std::back_inserter(functions));

  for(auto* f : functions) {
    std::cout << "Instrumenting " << f->name() << '\n';

    std::vector<pa::Point*> entryPoints;
    patch_manager->findPoints(pa::Scope(f), pa::Point::FuncEntry, std::back_inserter(entryPoints));

    for(auto* point : entryPoints) {
      std::cout << "  point at " << point->addr() << '\n';
      patcher.add(pa::PushBackCommand::create(point, snippet));
    }
  }

  patcher.commit();
}
