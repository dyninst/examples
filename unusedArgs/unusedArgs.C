// Example DataFlowAPI program; notes which arguments on x86_64 functions are unused
//
// William E. Cohen (wcohen at redhat dot com)
//

#include "Symtab.h"
#include "Function.h"
#include "liveness.h"

using namespace Dyninst;
using namespace SymtabAPI;
using namespace ParseAPI;
using namespace std;

#if defined(__x86_64__)
#  include "registers/x86_64_regs.h"
// Based on Figure 3.4: Register Usage of
// https://web.archive.org/web/20160801075146/http://www.x86-64.org/documentation/abi.pdf
const MachRegister arg_register[] = {x86_64::rdi, x86_64::rsi, x86_64::rdx,
                                     x86_64::rcx, x86_64::r8,  x86_64::r9};
#elif defined(__aarch64__)
#  include "registers/aarch64_regs.h"
// aarch64 calling conventions from https://developer.arm.com/documentation/ihi0055/d/
const MachRegister arg_register[] = {aarch64::x0, aarch64::x1, aarch64::x2, aarch64::x3,
                                     aarch64::x4, aarch64::x5, aarch64::x6, aarch64::x7};
#elif defined(__powerpc__)
#  include "registers/ppc32_regs.h"
#  include "registers/ppc64_regs.h"
// 64-bit powerpc calling conventions from
// https://refspecs.linuxfoundation.org/ELF/ppc64/PPC-elf64abi.html#REG
const MachRegister arg_register[] = {ppc64::r3, ppc64::r4, ppc64::r5, ppc64::r6,
                                     ppc64::r7, ppc64::r8, ppc64::r9, ppc64::r10};
#elif
#  error "Unsupported architecture"
#endif

int main(int argc, char** argv) {
  int num_reg_args = sizeof(arg_register) / sizeof(arg_register[0]);

  if(argc != 2)
    exit(-1);

  // Parse the object file
  Symtab* obj = NULL;
  bool err = Symtab::openFile(obj, argv[1]);

  if(err == false)
    exit(-1);

  // Create a new binary code object from the filename argument
  SymtabCodeSource* sts = new SymtabCodeSource(argv[1]);
  if(!sts)
    return -1;

  CodeObject* co = new CodeObject(sts);
  if(!co)
    return -1;

  // Iterate through each of the functions.
  for(auto f : co->funcs()) {
    // Perform the liveness analysis on function.
    LivenessAnalyzer la(f->obj()->cs()->getAddressWidth());
    la.analyze(f);

    // Get the first instruction of the first basic block (function entry).
    Block* bb = *f->blocks().begin();
    Address curAddr = bb->start();
    Instruction curInsn = bb->getInsn(curAddr);

    // Construct a liveness query location for the function entry.
    InsnLoc i(bb, curAddr, curInsn);
    Location loc(f, i);

    // Get the formal parameters and count them.
    SymtabAPI ::Function* func_sym;
    bool found = obj->findFuncByEntryOffset(func_sym, curAddr);
    if(!found)
      continue; // Missing symbols move on to next function
    vector<localVar*> parms;
    func_sym->getParams(parms);
    int num_parms = parms.size();

    // Output the results for the function
    cout << hex << curAddr << " " << f->name() << ": ";
    for(int i = 0; i < min(num_parms, num_reg_args); ++i) {
      // Print up arg number if associated register is unused.
      bool used;
      la.query(loc, LivenessAnalyzer::Before, arg_register[i], used);
      if(!used)
        cout << "arg" << i + 1 << " ";
    }
    cout << endl;
  }
  return (0);
}
