#include "Symtab.h"
#include "Type.h"
#include "Variable.h"

#include <iostream>
#include <vector>

int main() {
  namespace ds = Dyninst::SymtabAPI;
  ds::Symtab* sym;
  if(!ds::Symtab::openFile(sym, "libglobals.so")) {
    std::cout << "Failed to open 'libtest.so'\n";
    return -1;
  }

  for(auto var_name : {"x", "y", "z"}) {
    std::cout << "Looking for '" << var_name << "'\n";
    std::vector<ds::Variable*> vars;
    sym->findVariablesByName(vars, var_name);
    for(auto* v : vars) {
      std::cout << *v << "\n\n";
    }
    std::cout << "Found " << vars.size() << " variables named " << var_name << "\n";
    auto* v = vars[0];
    auto type = v->getType(ds::Type::do_share_t{});
    std::cout << "Type name: " << type->getName() << "\n";

    while(auto* arr = type->getArrayType()) {
      std::cout << "Name: " << arr->getName() << "\n"
                << "Bounds: " << arr->getLow() << "," << arr->getHigh() << "\n\n";
      type = arr->getBaseType(ds::Type::do_share_t{});
      if(!type)
        break;
    }
  }
}
