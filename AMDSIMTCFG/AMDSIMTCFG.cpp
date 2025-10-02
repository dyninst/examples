// Example ParseAPI program; produces a graph (in DOT format) of the
// control flow graph of the provided binary.
//
// Improvements by E. Robbins (er209 at kent dot ac dot uk)

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fmt/core.h>
#include <algorithm>

// instructionAPI
#include "Instruction.h"
// parseAPI
#include "CFG.h"
#include "CodeObject.h"

// dataflowAPI
#include "Graph.h"
#include "slicing.h"

namespace dp = Dyninst::ParseAPI;
namespace di = Dyninst::InstructionAPI;
using di::Instruction;
using fmt::print;
using std::cout , std::endl;

class ExecPred : public Dyninst::Slicer::Predicates{
  public:
    virtual bool endAtPoint(Dyninst::Assignment::Ptr ap){
      if(ap->insn().writesMemory())
        return true;
      return false;
    }
    virtual bool addPredecessor(Dyninst::AbsRegion reg){
      if(reg.absloc().type() == Dyninst::Absloc::Register) {
        return true;
      }
      return false;
    }
};

inline bool oprIsExec(di::Operand opr, Dyninst::Architecture arch){
  return opr.format(arch).find("EXEC") != std::string::npos;
}

bool insnWritesExec(di::Instruction & insn){
  std::vector<di::Operand> operands = insn.getAllOperands();
  Dyninst::Architecture arch = insn.getArch();
  for(auto opr : operands) {
    if(oprIsExec(opr,arch) && opr.isWritten()) {
      return true;        
    }
  }
  return false;
}

bool ReadsOrinsnWritesExec(di::Instruction & insn){
  std::vector<di::Operand> operands = insn.getAllOperands();
  Dyninst::Architecture arch = insn.getArch();
  for(auto opr : operands) {
    if(oprIsExec(opr,arch))
      return true;
  }
  return false;
}


void printNodesSorted(Dyninst::GraphPtr &slice){
  Dyninst::NodeIterator begin, end;
  slice->allNodes(begin,end);
  std::vector<Dyninst::SliceNode::Ptr> nodes; 
  if(begin == end)
    print("empty slice\n");
  else{
    for(Dyninst::NodeIterator ni = begin; ni != end; ni++){
      nodes.push_back(boost::dynamic_pointer_cast<Dyninst::SliceNode> (*ni));
    }
  }
  std::sort(nodes.begin(), nodes.end(), 
    [](const Dyninst::SliceNode::Ptr a, const Dyninst::SliceNode::Ptr b){
      return a->addr() < b->addr();
    }
  );

  print("\toutputting slice\n");
  for(auto node_it : nodes) {
      print("\t\t {} \n",node_it->format());
  }
  print("\tend of slice\n");
}

void mySlice(dp::Function * f){
  int num_blocks = 0;
  for (dp::Block * b : f->blocks()){
    num_blocks ++;
    dp::Block::Insns insns;
    b->getInsns(insns);
    for(auto iter : insns){
      di::Instruction instr = iter.second;
      Dyninst::Address addr = iter.first;
      std::vector<di::Operand> operands = instr.getAllOperands();
      if(insnWritesExec(instr)){
        print("Examing instruction {}\n",instr.format());
        Dyninst::AssignmentConverter ac(false,false);
        std::vector<Dyninst::Assignment::Ptr> assignments;
        ac.convert(instr, addr, f, b, assignments);
        for(uint32_t ai=0; ai < assignments.size(); ai++){
          std::string out_name = assignments[ai]->out().format();
          if(out_name.find("exec")==std::string::npos)
            continue;
          print("Slicing for output {}\n",out_name);
          Dyninst::Slicer execSlicer(assignments[ai],b,f,false,false);
          ExecPred pred;
          Dyninst::GraphPtr slice = execSlicer.backwardSlice(pred);
          printNodesSorted(slice);
        }
      }
    }
  } 
}


void processBlock(dp::Block * curB, std::vector<Dyninst::Address> &seenStack, 
    std::vector<dp::Block *> &execAlteringBlocks, std::unordered_set<Dyninst::Address> &seenAddr) {
  if(std::count(execAlteringBlocks.begin(),execAlteringBlocks.end(),curB)==0){
    seenAddr.insert(curB->start());
  }else{

    print("{}Block:0x{:x}\n",std::string(2*seenStack.size(),' '),curB->start());
    dp::Block::Insns insns;
    curB->getInsns(insns);
    for(auto iter : insns){
      Dyninst::Address addr = iter.first;
      di::Instruction instr = iter.second;
      if(ReadsOrinsnWritesExec(instr)){
        print("{}{:x} {}\n",std::string(2*seenStack.size(),' '),addr,instr.format());
      }
    }

  }
  seenStack.push_back(curB->start());
  for(dp::Edge * e : curB->targets()){
    if(std::count(seenStack.begin(),seenStack.end(),e->trg()->start())>0)
      continue;
    if(seenAddr.count(e->trg()->start())>0)
      continue;
    processBlock(e->trg(), seenStack, execAlteringBlocks,seenAddr);
  }
  seenStack.pop_back();
}

void mainProcess(dp::Function * f, std::vector<dp::Block * > &execAlteringBlocks) {
  std::vector<Dyninst::Address> seenStack;
  std::unordered_set<Dyninst::Address> seen;
  dp::Block * entry = f->entry();
  print("Entry block has {} edges\n",entry->targets().size()); 
  processBlock(entry,seenStack, execAlteringBlocks, seen);
}

void getExecAlteringBlocks(dp::Function * f, std::vector<dp::Block * > & blockSet){
  int num_blocks = 0;
  for (dp::Block * b : f->blocks()){
    num_blocks ++;
    dp::Block::Insns insns;
    b->getInsns(insns);
    bool writesExec = false;
    for(auto iter : insns){
      Dyninst::Address addr = iter.first;
      di::Instruction instr = iter.second;
      if(insnWritesExec(instr)){
        writesExec = true;
        print("{:x} {}\n",addr,instr.format());
      }
    }
    if(writesExec)
      blockSet.push_back(b);
  } 
  print(" {} out of {} blocks alters exec mask\n", blockSet.size(), num_blocks);
}

int main(int argc, char* argv[]) {
  if(argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " executable\n";
    return -1;
  }

  auto* sts = new dp::SymtabCodeSource(argv[1]);
  auto* co = new dp::CodeObject(sts);

  // Parse the binary
  co->parse();

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
  for (auto f: all){

    print("Examining Function {}, staring at address 0x{:x} no return? {}\n",f->name(),f->addr(),f->retstatus()==dp::NORETURN);
    std::vector<dp::Block *> execAlteringBlocks;
    getExecAlteringBlocks(f, execAlteringBlocks);
    //mainProcess(f, execAlteringBlocks);
    mySlice(f);
    print("\n\n");
  }
}
