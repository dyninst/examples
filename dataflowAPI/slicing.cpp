#include "slicing.h"

#include "CFG.h"
#include "Instruction.h"

#include <vector>

namespace pa = Dyninst::ParseAPI;
namespace ia = Dyninst::InstructionAPI;
namespace df = Dyninst::DataflowAPI;

// We extend the default predicates to control when to stop slicing
class ConstantPred : public Dyninst::Slicer::Predicates {
public:
  // We do not want to track through memory writes
  bool endAtPoint(Dyninst::AssignmentPtr ap) override { return ap->insn().writesMemory(); }

  // We can treat PC as a constant as its value is the address of the
  // instruction
  bool addPredecessor(Dyninst::AbsRegion reg) override {
    if(reg.absloc().type() == Dyninst::Absloc::Register) {
      return !reg.absloc().reg().isPC();
    }
    return true;
  }
};

// Assume that block b in function f ends with an indirect jump.
void AnalyzeJumpTarget(pa::Function* f, pa::Block* b) {
  // Get the last instruction in this block, which should be a jump
  ia::Instruction insn = b->getInsn(b->last());

  // Convert the instruction to assignments
  // The first parameter means to cache the conversion results.
  // The second parameter means whether to use stack analysis to analyze stack
  // accesses.
  Dyninst::AssignmentConverter ac(true, false);
  std::vector<Dyninst::Assignment::Ptr> assignments;
  ac.convert(insn, b->last(), f, b, assignments);

  // An instruction can corresponds to multiple assignment.
  // Here we look for the assignment that changes the PC.
  Dyninst::Assignment::Ptr pcAssign;
  for(auto a : assignments) {
    Dyninst::AbsRegion const& out = a->out();
    if(out.absloc().type() == Dyninst::Absloc::Register && out.absloc().reg().isPC()) {
      pcAssign = a;
      break;
    }
  }

  // Create a Slicer that will start from the given assignment
  Dyninst::Slicer s(pcAssign, b, f);

  // We use the customized predicates to control slicing
  ConstantPred mp;
  Dyninst::GraphPtr slice = s.backwardSlice(mp);
}
