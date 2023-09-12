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
#include <iostream>
#include <set>
#include "CodeObject.h"
#include "InstructionDecoder.h"
using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace InstructionAPI;

int main(int argc, char **argv){
    if(argc != 2){
        printf("Usage: %s <binary path>\n", argv[0]);
        return -1;
    }
    char *binaryPath = argv[1];

    SymtabCodeSource *sts;
    CodeObject *co;
    SymtabAPI::Symtab *symTab;
    std::string binaryPathStr(binaryPath);
    bool isParsable = SymtabAPI::Symtab::openFile(symTab, binaryPathStr);
    if(isParsable == false){
        const char *error = "error: file can not be parsed";
        cout << error;
        return - 1;
    }
    sts = new SymtabCodeSource(binaryPath);
    co = new CodeObject(sts);
    //parse the binary given as a command line arg
    co->parse();

    //get list of all functions in the binary
    const CodeObject::funclist &all = co->funcs();
    if(all.size() == 0){
        const char *error = "error: no functions in file";
        cout << error;
        return - 1;
    }
    auto fit = all.begin();
    std::set<Address> bb_addrs;
    Function *f = *fit;
    //create an Instruction decoder which will convert the binary opcodes to strings
    InstructionDecoder decoder(f->isrc()->getPtrToInstruction(f->addr()),
            InstructionDecoder::maxInstructionLength,
            f->region()->getArch());
    Instruction instr;;
    for(;fit != all.end(); ++fit){
        Function *f = *fit;
        //get address of entry point for current function
        //
        auto bit = f->blocks().begin();
        for(; bit != f->blocks().end(); bit++){
            Block * b = * bit;
            bb_addrs.insert(b->start());
        }
    }
    fit = all.begin();
    for(;fit != all.end(); ++fit){
        Function *f = *fit;
        //get address of entry point for current function
        //
        auto bit = f->blocks().begin();
        for(; bit != f->blocks().end(); bit++){
            Block * b = * bit;
            for (auto & edges : b->targets() ){
                Address branch_addr = edges->src()->lastInsnAddr();
                if( edges->type() == COND_TAKEN || edges->type() == DIRECT || edges->type() == CALL){
                    void * insn_ptr = f->isrc()->getPtrToInstruction(branch_addr);
                    instr = decoder.decode((unsigned char * ) insn_ptr);
                    std::locale loc;
                    std::string instr_str = instr.format(branch_addr);
                    cout << std::hex << branch_addr << " : ";
                    for (std::string::size_type i =0 ; i < instr_str.length(); i++){
                        cout << std::tolower(instr_str[i],loc) ;
                    }
                    cout << " ( " << edges->trg()->start() << " ) " ;
                    vector<Operand> operands;
                    instr.getOperands(operands);
                    Address target_addr = edges->trg()->start();
                    if(bb_addrs.find(target_addr) == bb_addrs.end()){
                        cout << " Address = " << operands[0].format(f->region() -> getArch(), branch_addr) << " doesn't match start of a BB";
                    }
                    //cout << " Address = " << operands[0].format(f->region() -> getArch(), branch_addr) << " ";
                    //cout << " AST = " << operands[0].format(f->region() -> getArch(), 0) << " ";
                    cout << endl;
                }
            }
        }
        //cout << endl;
    }
    return 0;
}
