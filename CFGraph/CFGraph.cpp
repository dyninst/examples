// Example ParseAPI program; produces a graph (in DOT format) of the
// control flow graph of the provided binary.
//
// Improvements by E. Robbins (er209 at kent dot ac dot uk)

#include "CFG.h"
#include "CodeObject.h"

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace dp = Dyninst::ParseAPI;
namespace st = Dyninst::SymtabAPI;

int main(int argc, char* argv[]) {
  if(argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " executable\n";
    return -1;
  }

  auto* sts = new dp::SymtabCodeSource(argv[1]);
  auto* co = new dp::CodeObject(sts);

  // Parse the binary
  co->parse();

  std::cout << "digraph G {" << '\n';

  // Print the control flow graph
  dp::CodeObject::funclist all = co->funcs();

  // Remove compiler-generated and system functions
  {
    auto ignore = [&all](dp::Function const* f) {
      auto const& name = f->name();
      bool const starts_with_underscore = name[0] == '_';
      bool const ends_with_underscore = name[name.length() - 1] == '_';
      bool const is_dummy = name == "frame_dummy";
      bool const is_clones = name.find("tm_clones") != std::string::npos;
      return starts_with_underscore || ends_with_underscore || is_dummy || is_clones;
    };

    // 'funclist' is a std::set which has only const iterators
    auto i = all.begin();
    while(i != all.end()) {
      if(ignore(*i)) {
        i = all.erase(i);
      } else {
        ++i;
      }
    }
  }

  std::unordered_set<Dyninst::Address> seen;

  int cluster_index = 0;
  for(auto const* f : all) {
    // Make a cluster for nodes of this function
    std::cout << "\t subgraph cluster_" << cluster_index << " { \n\t\t label=\"" << f->name()
              << "\"; \n\t\t color=blue;" << '\n';

    std::cout << "\t\t\"" << std::hex << f->addr() << std::dec << "\" [shape=box";

    if(f->retstatus() == dp::NORETURN)
      std::cout << ",color=red";

    std::cout << "]" << '\n';

    // Label functions by name
    std::cout << "\t\t\"" << std::hex << f->addr() << std::dec << "\" [label = \"" << f->name() << "\\n"
              << std::hex << f->addr() << std::dec << "\"];" << '\n';

    std::stringstream edgeoutput;

    for(dp::Block* b : f->blocks()) {
      // Don't revisit blocks in shared code
      if(seen.count(b->start()) > 0)
        continue;
      seen.insert(b->start());

      std::cout << "\t\t\"" << std::hex << b->start() << std::dec << "\";" << '\n';

      for(dp::Edge* e : b->targets()) {
        if(!e)
          continue;
        std::string s;
        if(e->type() == dp::CALL)
          s = " [color=blue]";
        else if(e->type() == dp::RET)
          s = " [color=green]";

        // Store the edges somewhere to be printed outside of the cluster
        edgeoutput << "\t\"" << std::hex << e->src()->start() << "\" -> \"" << e->trg()->start() << "\"" << s
                   << '\n';
      }
    }
    // End cluster
    std::cout << "\t}" << '\n';

    // Print edges
    std::cout << edgeoutput.str() << '\n';
  }
  std::cout << "}" << '\n';
}
