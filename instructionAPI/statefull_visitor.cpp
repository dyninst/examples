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
 * A stateful visitor
 *
 * This visitor remembers the AST type that was most-recently visited.
 */

struct stateful_visitor : di::Visitor {
  bool foundReg{};
  bool foundImm{};
  bool foundBin{};
  bool foundDer{};

  void visit(di::BinaryFunction*) override { foundBin = true; }

  void visit(di::Immediate*) override { foundImm = true; }

  void visit(di::RegisterAST*) override { foundReg = true; }

  void visit(di::Dereference*) override { foundDer = true; }

  // clang-format off
  friend std::ostream& operator<<(std::ostream &os, stateful_visitor const& v) {
    std::cout << std::boolalpha
              << "  foundReg: " << v.foundReg << '\n'
              << "  foundImm: " << v.foundImm << '\n'
              << "  foundBin: " << v.foundBin << '\n'
              << "  foundDer: " << v.foundDer << '\n';
    return os;
  }

  // clang-format on
};

void print(di::Instruction const& insn) {
  stateful_visitor v;
  std::vector<di::Operand> operands;
  insn.getOperands(operands);

  std::cout << "instruction: " << insn.format() << '\n';
  for(auto const& o : operands) {
    std::cout << "operand '" << o.format(insn.getArch()) << "'\n";
    o.getValue()->apply(&v);
    std::cout << v << '\n';
  }
}

int main() {
  // clang-format off
  std::array<const unsigned char, 15> buffer = {
      0x05, 0xef, 0xbe, 0xad, 0xde,   // add eax, 0xDEADBEEF
      0x90,                           // 1-byte nop
      0x66, 0x0F, 0x1F, 0x84, 0x00,
      0x00, 0x00, 0x00, 0x00          // 9-byte nop (NOP DWORD ptr [rax + rax*1 + 0x00000000])

  };
  // clang-format on

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
