#include "BinaryFunction.h"
#include "Dereference.h"
#include "Expression.h"
#include "Immediate.h"
#include "Instruction.h"
#include "InstructionDecoder.h"
#include "Operand.h"
#include "Register.h"
#include "Visitor.h"

#include <array>
#include <iomanip>
#include <iostream>

namespace di = Dyninst::InstructionAPI;

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

int main() {
  std::array<const unsigned char, 8> buffer = {
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
  } while(i.isValid());
}
