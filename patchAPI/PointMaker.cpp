#include "CFG.h"
#include "CFGMaker.h"
#include "PatchCFG.h"
#include "PatchCommon.h"
#include "PatchMgr.h"

#include <iostream>

namespace dp = Dyninst::ParseAPI;
namespace pa = Dyninst::PatchAPI;

/*
 * A custom point generator
 *
 */

struct PointTracer : pa::Point {
  PointTracer(pa::Point::Type t, pa::PatchMgrPtr m, pa::PatchFunction* f) : pa::Point(t, m, f) {
    std::cout << "Making point for " << f->name() << '\n';
  }
};

class PointMakerTracer : public pa::PointMaker {
public:
  pa::Point* mkFuncPoint(pa::Point::Type t, pa::PatchMgrPtr m, pa::PatchFunction* f) override {
    return new PointTracer(t, m, f);
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
  PointMakerTracer point_tracer{};
  auto patch_manager = pa::PatchMgr::create(address_space, nullptr, &point_tracer);

  std::vector<pa::PatchFunction*> functions;
  patch_object->funcs(std::back_inserter(functions));

  for(auto* f : functions) {
    std::vector<pa::Point*> entryPoints;
    patch_manager->findPoints(pa::Scope(f), pa::Point::FuncEntry, std::back_inserter(entryPoints));
  }
}
