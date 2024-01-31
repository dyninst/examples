#include "SymEval.h"
#include "slicing.h"

namespace df = Dyninst::DataflowAPI;

// We extend the default ASTVisitor to check whether the AST is a constant
class ConstVisitor : public Dyninst::ASTVisitor {
public:
  bool resolved;
  Dyninst::Address target;

  ConstVisitor() : resolved(true), target(0) {}

  // We reach a constant node and record its value
  Dyninst::AST::Ptr visit(df::ConstantAST* ast) override {
    target = ast->val().val;
    return Dyninst::AST::Ptr();
  };

  // If the AST contains a variable
  // or an operation, then the control flow target cannot
  // be resolved through constant propagation
  Dyninst::AST::Ptr visit(df::VariableAST*) override {
    resolved = false;
    return Dyninst::AST::Ptr();
  };

  Dyninst::AST::Ptr visit(df::RoseAST* ast) override {
    resolved = false;

    // Recursively visit all children
    unsigned totalChildren = ast->numChildren();
    for(unsigned i = 0; i < totalChildren; ++i) {
      ast->child(i)->accept(this);
    }
    return Dyninst::AST::Ptr();
  };
};

Dyninst::Address ExpandSlice(Dyninst::GraphPtr slice, Dyninst::Assignment::Ptr pcAssign) {
  df::Result_t symRet;
  df::SymEval::expand(slice, symRet);

  // We get AST representing the jump target
  Dyninst::AST::Ptr pcExp = symRet[pcAssign];

  // We analyze the AST to see if it can actually be resolved by constant
  // propagation
  ConstVisitor cv;
  pcExp->accept(&cv);
  if(cv.resolved)
    return cv.target;
  return 0;
}
