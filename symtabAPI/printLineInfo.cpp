#include "LineInformation.h"
#include "Module.h"
#include "Symtab.h"

#include <iostream>
#include <utility>
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

  std::vector<st::Module*> modules;
  obj->getAllModules(modules);

  for(auto* m : modules) {
    std::cout << "Module '" << m->fileName() << "'\n";
    auto* info = m->getLineInformation();
    if(!info) {
      std::cout << "  No line info\n";
      continue;
    }
    for(auto li = info->begin(); li != info->end(); ++li) {
      st::Statement const* stmt = *li;
      std::cout << *stmt << "\n";
    }
  }
}
