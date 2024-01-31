#include "AddrSpace.h"
#include "PatchObject.h"

namespace pa = Dyninst::PatchAPI;

struct MyAddrSpace : pa::AddrSpace {

  Dyninst::Address malloc(pa::PatchObject*, size_t, Dyninst::Address) override {
    // do memory allocation here
    return 0;
  }

  bool write(pa::PatchObject*, Dyninst::Address, Dyninst::Address, size_t) override {
    // copy data from the address from_addr to the address to_addr
    return true;
  }

  bool realloc(pa::PatchObject*, Dyninst::Address, size_t) override { return true; }

  bool free(pa::PatchObject*, Dyninst::Address) override { return true; }
};
