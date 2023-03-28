#include "InstructionDecoder.h"
#include <array>
#include <iostream>
#include <iomanip>
#include <string>

namespace ia = Dyninst::InstructionAPI;
using id = ia::InstructionDecoder;
using ui = id::unknown_instruction;

ia::Instruction unknown_instruction_handler(id::buffer b, Dyninst::Address addr) {
  std::cout << "\n\nUnknown instruction encountered in byte sequence starting at address "
		    << std::hex << addr << ": [";
  auto *begin = b.start;
  while(begin != b.end) {
    std::cout << std::hex << static_cast<int>(*begin) << ' ';
    ++begin;
  }
  std::cout << "]\n";
  
  // Return a multi-byte pseudo-NOP sequence
  return {
    {e_nop, "nop", Dyninst::Architecture::Arch_x86_64},
    5,
    b.start,
    Dyninst::Architecture::Arch_x86_64
  };
}

int main() {
  std::array<const unsigned char, 9> buffer =
  {
    0x05, 0xef, 0xbe, 0xad, 0xde, // ADD eAX, 0xDEADBEEF
    0x50,                         // PUSH rAX
    0xd6,                         // Illegal instruction
    0x74, 0x10                    // JZ +0x10(8)
  };
  
  ia::InstructionDecoder decoder(
      buffer.data(), buffer.size(),
      Dyninst::Architecture::Arch_x86_64
  );
  
  ui::register_callback(&unknown_instruction_handler);
  
  ia::Instruction i;
  do
  {
    i = decoder.decode();
    if(i.isValid()) {
      auto const* beg = static_cast<unsigned char const*>(i.ptr());
      std::cout << std::hex << std::showbase;
      for(auto end = beg+i.size(); beg!=end; ++beg) {
        std::cout << static_cast<int>(*beg) << ' ';
      }
      std::cout << i.format() << "\n";
    }
  }
  while(i.isValid());
}
