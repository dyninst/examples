# Insert Snippet Example for AMDGPU GFX908


*  This branch requires the ![kpp_amdgpu_instrument](https://github.com/dyninst/dyninst/tree/kpp_amdgpu_instrument) branch of dyninst.


In this example, we show how to add a nop instruction
in front of every s_load_dwordx2 instruction.


After building the example, one can run

noop_snippet vectorAdd-gfx908 vectorAdd-gfx908.new 

to do the instrumentation on vectorAdd-gfx908 and generate 
an instrumented binary vectorAdd-gfx908.new

Expected disassembly output are provided in  vectorAdd-gfx908.s
and vectorAdd-gfx908.new.s respectively.

