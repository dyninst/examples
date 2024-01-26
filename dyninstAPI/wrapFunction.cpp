#include "BPatch.h"
#include "BPatch_function.h"
#include "BPatch_object.h"
#include "Symtab.h"

#include <iostream>
#include <string>
#include <vector>

namespace st = Dyninst::SymtabAPI;

/* The instrumented binary should have these two functions defined:
 *
 *  void* origMalloc(unsigned long size);
 *  void* fastMalloc(unsigned long size);
 *
*/

char const* orig_func = "origMalloc";
char const* replacement_func = "fastMalloc";

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " file\n";
    return -1;
  }

  BPatch bpatch;
  BPatch_binaryEdit* appBin = bpatch.openBinary(argv[1]);

  if(!appBin) {
    std::cerr << "Unable to open '" << argv[1] << "'\n";
    return -1;
  }

  BPatch_image* appImage = appBin->getImage();

  auto find_func = [appImage](std::string const& name) -> BPatch_function* {
    std::vector<BPatch_function*> funcs;
    appImage->findFunction("", funcs);
    if(funcs.size() != 1) {
      std::cerr << "Unable to find '" << name << "'\n";
      return nullptr;
    }
    return funcs[0];
  };

  BPatch_function* original_malloc = find_func(orig_func);
  if(!original_malloc) {
    return -1;
  }

  BPatch_function* fast_malloc = find_func(replacement_func);
  if(!fast_malloc) {
    return -1;
  }

  auto* symtab = st::convert(fast_malloc->getModule())->exec();

  std::vector<st::Symbol*> syms;
  symtab->findSymbol(syms, orig_func,
                     st::Symbol::ST_UNKNOWN,   // Don’t specify type
                     st::NameType::prettyName, // Look for demangled symbol name
                     false,                    // Not regular expression
                     false,                    // Don’t check case
                     true);                    // Include undefined symbols

  if(syms.size() != 1) {
    std::cerr << "Unable to find symbol for '" << orig_func << "'\n";
    return -1;
  }

  appBin->wrapFunction(original_malloc, fast_malloc, syms[0]);
}
