#include "InstructionDecoder.h"

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace ia = Dyninst::InstructionAPI;
using id = ia::InstructionDecoder;
using ui = id::unknown_instruction;

ia::Instruction unknown_instruction_handler(id::buffer b) {
  std::cout << "\nUnknown instruction encountered in byte sequence [";
  auto* begin = b.start;
  while(begin != b.end) {
    std::cout << std::hex << static_cast<int>(*begin) << ' ';
    ++begin;
  }
  std::cout << "]\n\n";

  // Return a multi-byte pseudo-NOP sequence
  return {{e_nop, "nop", Dyninst::Architecture::Arch_x86_64}, 5, b.start, Dyninst::Architecture::Arch_x86_64};
}

int main() {
  // The "unknown" instruction here starts with 0xd6 which is specified in the
  // Intel manual as an Illegal Instruction. We append four zero bytes to
  // simulate a 'long' NOP allowed by the Intel Dev Manual.
  std::array<const unsigned char, 13> buffer = {
      0x05, 0xef, 0xbe, 0xad, 0xde, // ADD eAX, 0xDEADBEEF
      0x50,                         // PUSH rAX
      0xd6, 0x00, 0x00, 0x00, 0x00, // Unknown instruction; length = 5 bytes
      0x74, 0x10                    // JZ +0x10(8)
  };

  std::cout << "Decoding buffer starting at address 0x" << std::hex
            << reinterpret_cast<std::uint64_t>(buffer.data()) << "\n\n";

  ia::InstructionDecoder decoder(buffer.data(), buffer.size(), Dyninst::Architecture::Arch_x86_64);

  ui::register_callback(&unknown_instruction_handler);

  ia::Instruction i;
  do {
    i = decoder.decode();
    if(i.isValid()) {
      auto const* beg = static_cast<unsigned char const*>(i.ptr());
      std::stringstream s;
      s << std::hex << std::showbase;
      for(auto end = beg + i.size(); beg != end; ++beg) {
        s << static_cast<int>(*beg) << ' ';
      }
      std::cout << std::left << std::setw(24) << s.str() << " | " << i.format() << "\n";
    }
  } while(i.isValid());
}
