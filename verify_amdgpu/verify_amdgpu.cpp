#include "BinaryFunction.h"
#include "Dereference.h"
#include "Expression.h"
#include "Immediate.h"
#include "Instruction.h"
#include "InstructionDecoder.h"
#include "Operand.h"
#include "Register.h"
#include "MultiRegister.h"
#include "Visitor.h"
#include "CodeObject.h"
#include <array>
#include <iomanip>
#include <iostream>

namespace di = Dyninst::InstructionAPI;
using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace InstructionAPI;
/*
 * A stateless visitor
 *
 *  This visitor simply accesses some property of the ASTs, but
 *  does not store any information about them.
 */

class printer : public di::Visitor {
public:
  void visit(di::BinaryFunction* b) override {
    std::cout << "  BinaryFunction '" << b->format(di::defaultStyle) << "'\n";
  };

  void visit(di::Immediate* i) override {
    std::cout << "  Immediate '0x" << i->format(di::defaultStyle) << "'\n";
  };

  void visit(di::Dereference* d) override {
    std::cout << "  Dereference '" << d->format(di::defaultStyle) << "'\n";
  };

  void visit(di::RegisterAST* r) override {
    std::cout << "  Register '" << r->format(di::defaultStyle) << "'\n";
  }

  void visit(di::MultiRegisterAST* r) override {
    std::cout << "  MultiRegister '" << r->format(di::defaultStyle) << "'\n";
  }

};

void print(di::Instruction const& insn) {
  printer pv;
  std::vector<di::Operand> operands;
  insn.getOperands(operands);

  std::cout << "instruction: " << insn.format() << '\n';
  int op_num = 1;
  for(auto const& o : operands) {
    std::cout << "operand" << op_num << " '" << o.format(insn.getArch()) << "'\n";
    o.getValue()->apply(&pv);
    op_num++;
  }
}

int main(int argc, char** argv) {
  if(argc != 2) {
    printf("Usage: %s <binary path>\n", argv[0]);
    return -1;
  }
  char* binaryPath = argv[1];

  SymtabCodeSource* sts;
  CodeObject* co;
  Instruction instr;
  SymtabAPI::Symtab* symTab;
  std::string binaryPathStr(binaryPath);
  bool isParsable = SymtabAPI::Symtab::openFile(symTab, binaryPathStr);
  if(isParsable == false) {
    const char* error = "error: file can not be parsed";
    cout << error;
    return -1;
  }
  sts = new SymtabCodeSource(binaryPath);
  co = new CodeObject(sts);
  // parse the binary given as a command line arg
  co->parse();

  // get list of all functions in the binary
  const CodeObject::funclist& all = co->funcs();
  if(all.size() == 0) {
    const char* error = "error: no functions in file";
    cout << error;
    return -1;
  }
  auto fit = all.begin();
  Function* f = *fit;
  // create an Instruction decoder which will convert the binary opcodes to strings
  InstructionDecoder decoder(f->isrc()->getPtrToInstruction(f->addr()),
                             InstructionDecoder::maxInstructionLength, f->region()->getArch());
  for(; fit != all.end(); ++fit) {
    Function* f = *fit;
    // get address of entry point for current function
    Address crtAddr = f->addr();
    int instr_count = 0;
    instr = decoder.decode((unsigned char*)f->isrc()->getPtrToInstruction(crtAddr));
    auto fbl = f->blocks().end();
    fbl--;
    Block* b = *fbl;
    Address lastAddr = b->end();
    // if current function has zero instructions, d o n t output it
    if(crtAddr == lastAddr)
      continue;
    cout << "\n\n\"" << f->name() << "\" :";
    while(crtAddr < lastAddr) {
      // decode current instruction
      instr = decoder.decode((unsigned char*)f->isrc()->getPtrToInstruction(crtAddr));
      print(instr);
      //cout << "\n" << hex << crtAddr;
      //cout << ": \"" << instr.format() << "\"";
      // go to the address of the next instruction
      crtAddr += instr.size();
      instr_count++;
    }
  }
  cout << "\n";  
  /*std::array<const unsigned char, 8> buffer = {
      0x05, 0xef, 0xbe, 0xad, 0xde, // add eax, 0xDEADBEEF
      0x50,                         // push rax
      0x74, 0x10                    // je  0x12
  };

  di::InstructionDecoder decoder(buffer.data(), buffer.size(), Dyninst::Architecture::Arch_x86_64);

  di::Instruction i;
  do {
    i = decoder.decode();
    if(i.isValid()) {
      print(i);
      std::cout << '\n';
    }
  } while(i.isValid());*/
  return 0;
}
