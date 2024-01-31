#include "BPatch.h"
#include "BPatch_function.h"
#include "BPatch_image.h"
#include "BPatch_point.h"
#include "BPatch_process.h"
#include "BPatch_thread.h"
#include "BPatch_type.h"

#include <iostream>

BPatch_process* appThread;
BPatch_image* appImage;
BPatch_type* intType;

BPatch_function* traceEntryFunc;
BPatch_function* traceCallSiteFunc;
BPatch_function* traceExitFunc;

void showModsFuncs(BPatch_process* appThread);

BPatch_function* findFunc(char const* str) {
  BPatch_Vector<BPatch_function*>* funcBuf = new BPatch_Vector<BPatch_function*>;
  appImage->findFunction(str, *funcBuf);
  if((*funcBuf).size() < 1) {
    std::cerr << "couldn't find the function " << str << "\n";
    exit(1);
  }

  return (*funcBuf)[0]; // grab an arbitrary one
}

void initTraceLibrary() {
  const char* tracetool_lib = getenv("TRACETOOL_LIB");
  if(tracetool_lib == nullptr) {
    std::cerr << "Need to set environment variable TRACETOOL_LIB to use "
              << "tracetool\n";
    exit(1);
  }

  if(!appThread->loadLibrary(tracetool_lib)) {
    std::cerr << "failed when attempting to load library " << tracetool_lib << std::endl;
    exit(1);
  }

  traceEntryFunc = findFunc("trace_entry_func");
  traceCallSiteFunc = findFunc("trace_callsite_func");
  traceExitFunc = findFunc("trace_exit_func");
}

enum arg_type { tr_unknown = 0, tr_int = 1 };

void instrument_entry(BPatch_function* func, char* funcname) {
  BPatch_Vector<BPatch_localVar*>* params = func->getParams();
  int num_args = (*params).size();
  enum arg_type argOneType = tr_unknown;

  if(num_args > 0) {
    BPatch_localVar* firstParam = (*params)[0];
    BPatch_type* first_arg_type = firstParam->getType();
    if(first_arg_type && first_arg_type->getID() == intType->getID())
      argOneType = tr_int;
  }

  BPatch_Vector<BPatch_snippet*> traceFuncArgs;
  BPatch_constExpr funcName(funcname);
  BPatch_constExpr descArg("...desc...");
  BPatch_constExpr numFuncArgs(num_args);
  BPatch_paramExpr argOne(0);
  BPatch_constExpr nullptrArgOne(static_cast<void*>(nullptr));
  BPatch_constExpr argType(argOneType);

  traceFuncArgs.push_back(&funcName);
  traceFuncArgs.push_back(&descArg);
  traceFuncArgs.push_back(&numFuncArgs);
  if(num_args > 0)
    traceFuncArgs.push_back(&argOne);
  else
    traceFuncArgs.push_back(&nullptrArgOne);
  traceFuncArgs.push_back(&argType);

  BPatch_Vector<BPatch_point*>* entryPointBuf = func->findPoint(BPatch_entry);
  if((*entryPointBuf).size() != 1) {
    std::cerr << "couldn't find entry point for func " << funcname << std::endl;
    exit(1);
  }
  BPatch_point* entryPoint = (*entryPointBuf)[0];

  BPatch_funcCallExpr traceEntryCall(*traceEntryFunc, traceFuncArgs);
  appThread->insertSnippet(traceEntryCall, *entryPoint, BPatch_callBefore, BPatch_firstSnippet);
}

void instrument_callsite(char* callee_funcname, BPatch_function* callsite_func,
                         BPatch_point* callsite_point) {
  BPatch_Vector<BPatch_localVar*>* params = callsite_func->getParams();
  int num_args = 0;
  if(params != nullptr)
    num_args = (*params).size();
  enum arg_type argOneType = tr_unknown;

  if(num_args > 0) {
    BPatch_localVar* firstParam = (*params)[0];
    BPatch_type* first_arg_type = firstParam->getType();
    if(first_arg_type && first_arg_type->getID() == intType->getID())
      argOneType = tr_int;
  }

  char callsite_func_name[100];
  callsite_func->getName(callsite_func_name, 99);

  BPatch_Vector<BPatch_snippet*> traceFuncArgs;
  BPatch_constExpr callee_funcName(callee_funcname);
  BPatch_constExpr callsite_funcName(callsite_func_name);
  BPatch_constExpr descArg("...desc...");
  BPatch_constExpr numFuncArgs(num_args);
  BPatch_paramExpr argOne(0);
  BPatch_constExpr nullptrArgOne((const void*)nullptr);
  BPatch_constExpr argType(argOneType);

  // Currently on Solaris, within instrumentation can't call functions
  // with more than 5 arguments

  // traceFuncArgs.push_back(&callee_funcName);
  traceFuncArgs.push_back(&callsite_funcName);
  traceFuncArgs.push_back(&descArg);
  traceFuncArgs.push_back(&numFuncArgs);

  if(num_args > 0)
    traceFuncArgs.push_back(&argOne);
  else
    traceFuncArgs.push_back(&nullptrArgOne);
  traceFuncArgs.push_back(&argType);

  BPatch_funcCallExpr traceCallSiteCall(*traceCallSiteFunc, traceFuncArgs);
  appThread->insertSnippet(traceCallSiteCall, *callsite_point, BPatch_callBefore, BPatch_firstSnippet);
}

void instrument_callsites(BPatch_function* func, char* callee_funcname) {
  BPatch_Vector<BPatch_point*>* callsites = func->findPoint(BPatch_subroutine);

  for(auto* callsite_point : *callsites) {
    BPatch_function* callsite_func = callsite_point->getCalledFunction();
    if(callsite_func == nullptr) {
      std::cerr << "    skipping instrumentation for callsite in function \n"
                << "    " << callee_funcname << " since can't determine callee at callsite\n";
      continue;
    }
    instrument_callsite(callee_funcname, callsite_func, callsite_point);
  }
}

void instrument_exit(BPatch_function* func, char* funcname) {
  std::cerr << "calling instrument_exit\n";
  BPatch_type* retType = func->getReturnType();

  enum arg_type argOneType = tr_unknown;
  if(retType && retType->getID() == intType->getID())
    argOneType = tr_int;

  BPatch_Vector<BPatch_snippet*> traceFuncArgs;
  BPatch_constExpr funcName(funcname);
  BPatch_constExpr descArg("...desc...");
  BPatch_retExpr retVal;
  BPatch_constExpr retValType(argOneType);

  traceFuncArgs.push_back(&funcName);
  traceFuncArgs.push_back(&descArg);
  traceFuncArgs.push_back(&retVal);
  traceFuncArgs.push_back(&retValType);

  BPatch_Vector<BPatch_point*>* exitPointBuf = func->findPoint(BPatch_exit);

  // dyninst might not be able to find exit point
  if((*exitPointBuf).size() == 0) {
    std::cerr << "   couldn't find exit point, so returning\n";
    return;
  }

  for(auto* curExitPt : *exitPointBuf) {
    std::cerr << "   inserting an exit pt instrumentation\n";

    BPatch_funcCallExpr traceExitCall(*traceExitFunc, traceFuncArgs);
    appThread->insertSnippet(traceExitCall, *curExitPt, BPatch_callAfter, BPatch_firstSnippet);
  }
}

void instrument_funcs_in_module(BPatch_module* mod) {
  BPatch_Vector<BPatch_function*>* allprocs = mod->getProcedures();

  char name[100];
  for(unsigned i = 0; i < (*allprocs).size(); i++) {
    BPatch_function* func = (*allprocs)[i];
    func->getName(name, 99);

    std::cout << "  instrumenting function #" << i + 1 << ":  " << name << std::endl;
    instrument_entry(func, name);
    instrument_exit(func, name);
    instrument_callsites(func, name);
  }
}

// clang-format off
void usage() {
  fprintf(stderr, "Usage: tracetool [-p<pid>] program [prog-arguments]\n");
  fprintf(stderr, "       -p: specify process id of program, for attaching\n");
  fprintf(stderr,
          "       if no pid is passed, then the process is started directly\n");
  fprintf(stderr, "       (need to set environment variable TRACETOOL_LIB to "
                  "\n        path of trace library)\n");
}

// clang-format on

void postForkFunc(BPatch_thread* parent, BPatch_thread* child) {
  std::cerr << "###############################################################\n";
  std::cerr << "tool:  a fork occurred, parent pid: " << parent->getProcess()->getPid()
            << ", child pid: " << child->getProcess()->getPid() << std::endl;
  std::cerr << "###############################################################\n";
  parent->getProcess()->continueExecution();
  child->getProcess()->continueExecution();
  std::cerr << "done with postForkFunc\n";
}

// End the sequence with a nullptr
// should_instrument_module is expecting these to all be in lowercase
char const* excluded_modules[] = {
    "default_module", "libstdc++", "libm",   "libc", "ld-linux", "libdyninstapi_rt",
    "libdl",          "tracelib",  "kernel", ".so.",
    "global_linkage", // AIX modules
    nullptr};

void mystrlwr(char* str) {
  for(char* c = str; (*c) != 0; c++) {
    *c = tolower(*c);
  }
}

bool should_instrument_module(char const* mod_input) {
  int i = 0;
  char modname[100];
  strcpy(modname, mod_input);
  mystrlwr(modname);
  while(i < 1000) {
    char const* cur_mod = excluded_modules[i];
    if(cur_mod == nullptr)
      break;
    if(strstr(modname, cur_mod))
      return false;
    i++;
  }
  return true;
}

void handleArguments(int argc, char* argv[], bool* show_usage, int* pid, char** program,
                     char const* prog_args[]) {
  *pid = 0;
  *show_usage = false;
  // start at argument 1, that is skip the program name, because we don't
  // care about the program name
  if(argc <= 1) {
    *show_usage = true;
    return;
  }

  int parg_index = 0;
  bool have_prog = false;
  for(int i = 1; i < argc; i++) {
    char* curArg = argv[i];

    if(curArg[0] == '-' && curArg[1] == 'h') {
      *show_usage = true;
      return;
    } else if(curArg[0] == '-' && curArg[1] == 'p') {
      *pid = atoi(&curArg[2]);
    } else {
      if(have_prog == false) {
        *program = curArg;
        have_prog = true;
      }
      prog_args[parg_index] = curArg;
      parg_index++;
    }
  }
  prog_args[parg_index] = nullptr;
}

int main(int argc, char* argv[]) {
  bool show_usage;
  int pid = 0;

  char const* prog_args[50]; // maximum of 50 program arguments
  char* program_path{};

  handleArguments(argc, argv, &show_usage, &pid, &program_path, prog_args);
  if(show_usage) {
    usage();
    return 1;
  }

  BPatch bpatch;
  bpatch.registerPostForkCallback(postForkFunc);

  if(pid > 0) { // attach case
    std::cerr << "Attaching to process " << program_path << " with pid " << pid << std::endl;
    appThread = bpatch.processAttach(program_path, pid);
  } else { // create case
    std::cerr << "Creating process " << program_path << " with specified args\n";
    appThread = bpatch.processCreate(program_path, prog_args);
  }

  appImage = appThread->getImage();

  initTraceLibrary();
  intType = appImage->findType("int");

  const BPatch_Vector<BPatch_module*>* mbuf = appImage->getModules();

  for(unsigned n = 0; n < (*mbuf).size(); n++) {
    BPatch_module* mod = (*mbuf)[n];
    char modname[100];
    mod->getName(modname, 99);
    std::cout << "Program Module " << modname << " ------------------" << std::endl;
    if(should_instrument_module(modname)) {
      instrument_funcs_in_module(mod);
    }
  }

  std::cerr << "done instrumenting\n";
  std::cerr << "=====================================================\n";
  appThread->continueExecution();

  /* the rest of the execution occurs in postForkFunc() */
  while(bpatch.waitForStatusChange()) {
    if(appThread->isTerminated()) {
      std::cerr << "==> process pid " << appThread->getPid() << " has exited\n";
      break;
    } else if(appThread->isStopped()) {
      std::cerr << "==> process pid " << appThread->getPid() << " is stopped\n";
      break;
    }
  }
}
