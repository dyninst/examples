/*
 * Copyright (c) 1996-2011 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


/* 
 * A simple tool to perform library fingerprinting on a stripped binary.
 *
 * This tool uses parseAPI and symtabAPI to locate library wrapper
 * functions, identify them using a descriptor database, and output a
 * new binary with these new labels added to the symbtol table.
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include <fstream>
#include <set>
#include <string>
#include <vector>

// parseAPI
#include "CodeSource.h"
#include "CodeObject.h"
#include "CFG.h"

// symtabAPI
#include "Function.h"

// instructionAPI
#include "InstructionDecoder.h"
#include "Register.h"
#include "Dereference.h"
#include "Immediate.h"
#include "Result.h"

// dataflowAPI
#include "SymEval.h"
#include "slicing.h"

using namespace std;
using namespace Dyninst;
using namespace InstructionAPI;
using namespace ParseAPI;

class MyPred : public Slicer::Predicates{
    public:
        virtual bool endAtPoint(Assignment::Ptr ap){
            printf("backward slicing to address 0x%lx\n", ap->addr());
            if (ap->insn().getOperation().getID() == amdgpu_gfx908_op_S_WAITCNT){
                return true;
            }
            return false;
        }
};

int main(int argc, char **argv){
    if(argc != 2){
        printf("Usage: %s <binary path>\n", argv[0]);
        return -1;
    }
    char *binaryPath = argv[1];

    SymtabCodeSource *sts;
    CodeObject *co;
    Instruction instr;
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
    Function *f = *fit;
    //create an Instruction decoder which will convert the binary opcodes to strings
    InstructionDecoder decoder(f->isrc()->getPtrToInstruction(f->addr()),
            InstructionDecoder::maxInstructionLength,
            f->region()->getArch());

/*
    InstructionDecoder decoder(buf, InstructionDecoder::maxInstructionLength, block->obj()->cs()->getArch());

    Instruction insn = decoder.decode();
    AssignmentConverter ac(true, false);
    vector<Assignment::Ptr> assignments;
    ac.convert(insn, block->last(), func, block, assignments);

    Slicer formatSlicer(assignments[0], block, func, false, false);

    SymbolicExpression se;
    se.cs = block->obj()->cs();
    se.cr = block->region();
    JumpTableFormatPred jtfp(func, block, rf, thunks, se);

    GraphPtr slice = formatSlicer.backwardSlice(jtfp);
*/


    for(;fit != all.end(); ++fit){
        Function *f = *fit;
        //get address of entry point for current function
        auto bit = f->blocks().begin();
        for (; bit != f->blocks().end(); bit++){
            Block * blk = *bit;
            
            Address crtAddr = blk->start();
            
            Address lastAddr = blk->end();
            if(crtAddr == lastAddr)
                continue;

            printf("block starting at addr %lx, end at addr %lx\n",crtAddr,lastAddr);
            while(crtAddr < lastAddr){
                //decode current instruction
                instr = decoder.decode((unsigned char *)f->isrc()->getPtrToInstruction(crtAddr));

                if(instr.getOperation().getID() == amdgpu_gfx908_op_S_WAITCNT || instr.getOperation().getID() == amdgpu_gfx908_op_S_ENDPGM ){

                    printf("Found waicnt instruction at addr : 0x%lx\n", crtAddr );
                    cout << "\n" << hex << crtAddr;
                    cout << ": \"" << instr.format() << "\"\n";

                    AssignmentConverter ac(false, false);
                    vector<Assignment::Ptr> assignments;
                    ac.convert(instr, crtAddr, f, blk, assignments);
                    for (uint32_t ai = 0; ai < assignments.size(); ai++){
                        Slicer formatSlicer(assignments[ai], blk, f, false, false);

                        MyPred mp;
                        GraphPtr slice  = formatSlicer.backwardSlice(mp); 
                        NodeIterator begin,end;
                        slice->allNodes(begin,end);
                        if(begin == end){
                            printf("empty slice\n");
                        }
                        printf("\tOutputting Slice\n");
                        for(NodeIterator ni = begin; ni != end; ni++){
                           printf("\t\t %s \n",(*ni)->format().c_str());
                        }
                        printf("\tEnd Slice\n"); 
                    }
                }
                                //go to the address of the next instruction*/
                crtAddr += instr.size();
            }
            printf("end of block\n\n");

        }
    }

    return 0;
}
