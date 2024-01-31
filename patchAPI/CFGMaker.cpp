#include "CFGMaker.h"

#include "CFG.h"
#include "PatchCFG.h"

#include <iostream>

namespace dp = Dyninst::ParseAPI;
namespace pa = Dyninst::PatchAPI;

/*
 * A custom function CFG generator
 *
 */

struct FunctionTracer : pa::PatchFunction {
  FunctionTracer(dp::Function* f, pa::PatchObject* o) : pa::PatchFunction(f, o) {
    std::cout << "Making function " << f->name() << '\n';
  }
};

class CFGTracer : public pa::CFGMaker {
public:
  pa::PatchFunction* makeFunction(dp::Function* f, pa::PatchObject* o) override {
    return new FunctionTracer(f, o);
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
  CFGTracer tracer;
  auto* obj = pa::PatchObject::create(co, base_address, &tracer);

  std::vector<pa::PatchFunction*> all_funcs;
  obj->funcs(back_inserter(all_funcs));

  // suppress compiler warning
  (void)all_funcs;
}
