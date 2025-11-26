#!/bin/bash
filename=$1
llvm-objdump -d --no-show-raw-insn $1 > ./res.llvm.out  #~/issue-2050/parseapi/libparseAPI.so
./compareLLVM $1 > ./res.dyninst.out  #~/issue-2050/parseapi/libparseAPI.so
./parser.py res.llvm.out res.dyninst.out

