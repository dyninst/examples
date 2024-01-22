#include "Symbol.h"
#include "Symtab.h"

#include <iostream>
#include <vector>

namespace st = Dyninst::SymtabAPI;

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " file\n";
    return -1;
  }

  auto file_name = argv[1];

  st::Symtab* obj{};

  if(!st::Symtab::openFile(obj, file_name)) {
    std::cerr << "Unable to open " << file_name << '\n';
    return -1;
  }

  // Local variables in function 'bar'
  std::vector<st::Function*> funcs;
  obj->findFunctionsByName(funcs, "bar");

  for(auto* f : funcs) {
    std::cout << f->getName() << '\n';
    std::vector<st::localVar*> local_vars;
    f->getLocalVariables(local_vars);
    for(auto* v : local_vars) {
      std::cout << v->getName() << "\n";
    }
  }

  // All local variables name 'x'
  std::vector<st::localVar*> vars;
  obj->findLocalVariable(vars, "x");

  // Print the function and file they came from
  for(auto* v : vars) {
    for(auto loc : v->getLocationLists()) {
      st::Function* f;
      obj->getContainingFunction(loc.lowPC, f);
      // clang-format off
      std::cout << f->getName() << ", "
                << v->getName() << ", "
                << "[" << loc.lowPC << ", " << loc.hiPC << "] "
                << v->getFileName() << "[" << v->getLineNum() << "]\n";
      // clang-format on
    }
  }
}
