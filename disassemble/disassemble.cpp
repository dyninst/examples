/*
   Copyright (C) 2015 Alin Mindroc
   (mindroc dot alin at gmail dot com)

   This is a sample program that shows how to use InstructionAPI in order to
   print the assembly code and functions in a provided binary.
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/
#include "CodeObject.h"
#include "InstructionDecoder.h"

#include <iostream>
#include <iomanip>
using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace InstructionAPI;

// How many bytes of the instruction hex dump should be printed
// on the first line. The remaining will go to the second line
// on the assumption that an instruction is at most 15 bytes long.
static const int l1_width = 7;

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
  // create an Instruction decoder which will convert the binary opcodes to strings
  InstructionDecoder decoder((const void *)nullptr, 1, sts->getArch());
  for(auto fit = all.begin(); fit != all.end(); ++fit) {
    Function* f = *fit;
    // get address of entry point for current function
    Address crtAddr = f->addr();
    int instr_count = 0;
    auto fbl = f->blocks().end();
    fbl--;
    Block* b = *fbl;
    Address lastAddr = b->end();
    // if current function has zero instructions, d o n t output it
    if(crtAddr == lastAddr)
      continue;
    cout << "\n\n" << hex << setfill('0') << setw(2 * sts->getAddressWidth()) << f->addr() << " <" << f->name() << ">:\n";
    while(crtAddr < lastAddr) {
      // decode current instruction
      const unsigned char *instr_ptr = (const unsigned char *)f->isrc()->getPtrToInstruction(crtAddr);
      instr = decoder.decode(instr_ptr);

      // failed to decode the instruction
      if (instr.size() == 0)
        break;

      // pretty print it
      cout << hex << setfill(' ') << setw(8) << crtAddr << ":       ";
      for (size_t i = 0; i < instr.size() && i < l1_width; i++) {
        cout << hex << setfill('0') << setw(2) << (unsigned)instr_ptr[i] << " ";
      }
      for (size_t i = min(instr.size(), (size_t)l1_width); i < 8; i++) {
        cout << "   ";
      }
      cout << instr.format() << "\n";
      if (instr.size() > l1_width) {
        cout << hex << setfill(' ') << setw(8) << crtAddr + l1_width << ":       ";
        for (size_t i = l1_width; i < instr.size(); i++) {
          cout << hex << setfill('0') << setw(2) << (unsigned)instr_ptr[i] << " ";
        }
        cout << "\n";
      }

      // go to the address of the next instruction
      crtAddr += instr.size();
      instr_count++;
    }
  }
  return 0;
}
