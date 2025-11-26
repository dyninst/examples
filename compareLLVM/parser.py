#!/usr/bin/python3

import sys

llvm_fname = sys.argv[1]
dyn_fname = sys.argv[2]
llvm_addrs = dict()
dyn_addrs = dict()

def parse_func(llvmlines,llvmit,cutoff,addr_map):
    nameline = llvmlines[llvmit]
    llvmit+=1
    #print("Start of parsing ",nameline,llvmit)
    while llvmit < cutoff and llvmlines[llvmit].strip() != "":
        line = llvmlines[llvmit].strip()
        if line.startswith("#"):
            llvmit+=1
            continue
        sep_index = line.index(":")
        addr = int(line[:sep_index],16)
        command = line[sep_index+1:]
        addr_map[addr] = command
        llvmit+=1
    #print("End of parsing ",nameline,llvmit)
    return llvmit

def parse_llvm(fname):
    print("opening file",fname)
    llvmlines = open(fname,"r").readlines()
    llvmlen = len(llvmlines)
    print(llvmlen)
    llvmit = 3
    secheader = "Disassembly of section"
    while llvmit < llvmlen:
        #print("Accessing llvmit", llvmit,len(llvmlines))
        if llvmlines[llvmit].startswith(secheader):
            llvmit+=2
        else:
            llvmit = parse_func(llvmlines,llvmit,llvmlen,llvm_addrs)
            llvmit+=1

def parse_dyninst(fname):
    print("opening file",fname)
    dynlines = open(fname,"r").readlines()
    dynlen = len(dynlines)
    print(dynlen)
    dynit = 2
    while dynit < dynlen:
        dynit = parse_func(dynlines,dynit,dynlen,dyn_addrs)
        dynit+=2

def output_def(llvm_addrs,dyn_addrs):
    print("Collected %d llvm addrs"%len(llvm_addrs.keys()))
    print("Collected %d dyninst addrs"%len(dyn_addrs.keys()))
    AnB = llvm_addrs.keys() - dyn_addrs.keys()
    BnA = dyn_addrs.keys() - llvm_addrs.keys()
    print("Number of address unique to llvm = %d"%len(AnB))
    for addr in sorted(AnB):
        print("%x : %s"%(addr,llvm_addrs[addr]))

parse_llvm(llvm_fname)
parse_dyninst(dyn_fname)
output_def(llvm_addrs,dyn_addrs)

