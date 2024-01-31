#include "PatchModifier.h"

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
 * Remove a function from the CFG.
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

  pa::Patcher patcher(patch_manager);

  std::vector<pa::PatchFunction*> functions;
  patch_object->funcs(std::back_inserter(functions));

  auto const* func_to_remove = "InterestingProcedure";
  auto found = std::find_if(functions.begin(), functions.end(),
                            [func_to_remove](pa::PatchFunction* p) { return p->name() == func_to_remove; });

  if(found == functions.end()) {
    std::cerr << "Couldn't find '" << func_to_remove << "'\n";
    return -1;
  }

  auto print_functions = [](std::vector<pa::PatchFunction*> const& funcs) {
    for(auto* f : funcs) {
      std::cout << f->name() << '\n';
    }
  };

  print_functions(functions);

  pa::PatchModifier::remove(*found);

  functions.clear();
  patch_object->funcs(std::back_inserter(functions));

  print_functions(functions);
}
