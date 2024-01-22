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

  // search for a function with demangled (pretty) name "bar".
  std::vector<st::Function*> funcs;
  if(!obj->findFunctionsByName(funcs, "bar")) {
    std::cerr << "Didn't find function 'bar' in " << file_name << '\n';
  }

  for(auto* f : funcs) {
    std::cout << f->getName() << '\n';
  }

  // search defined symbols for one with a mangled name like "bar".
  std::vector<st::Symbol*> syms;
  constexpr bool is_regex = true;
  constexpr auto symbol_type = st::Symbol::ST_UNKNOWN;
  constexpr auto name_type = st::NameType::mangledName;
  constexpr bool case_match = false;
  constexpr bool include_undefined = false;
  if(!obj->findSymbol(syms, "bar", symbol_type, name_type, is_regex, case_match, include_undefined)) {
    std::cerr << "Didn't any symbol 'bar' in " << file_name << '\n';
  }

  for(auto* s : syms) {
    std::cout << s->getMangledName() << '\n';
  }
}
