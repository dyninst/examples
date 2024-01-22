#include "Symbol.h"
#include "Symtab.h"

#include <iostream>
#include <vector>

namespace st = Dyninst::SymtabAPI;

std::vector<st::Variable*> find_all_variables(st::Module* m) {
  std::vector<st::Variable*> vars;
  constexpr auto name_type = st::NameType::anyName;
  constexpr bool is_regex = true;
  constexpr bool case_match = false;
  m->findVariablesByName(vars, "*", name_type, is_regex, case_match);
  return vars;
}

void print_module_vars(std::vector<st::Module*> const& modules) {
  for(auto* m : modules) {
    std::cout << "Module '" << m->fileName() << "'\n";
    for(auto* v : find_all_variables(m)) {
      std::cout << *v << '\n';
    }
    std::cout << "\n\n";
  }
}

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

  std::vector<st::Module*> modules;
  obj->getAllModules(modules);

  std::cout << "**** BEFORE ****\n";
  print_module_vars(modules);
  std::cout << "*****************\n\n";

  for(auto* m : modules) {
    obj->createVariable("newIntVar", // Name of new variable
                        0x12345,     // Offset from data section
                        sizeof(int), // Size of symbol
                        m);
  }

  std::cout << "**** AFTER ****\n";
  print_module_vars(modules);
  std::cout << "***************\n";
}
