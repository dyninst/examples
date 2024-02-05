# Instrument Memory Access

## A tool for counting the number of times a load or store is performed

This is similar to Memory Access Counter, but
**all** functions in the binary are instrumented here.

This example illustrates how to use Dyninst to iterate over a
function’s control flow graph and inspect instructions. These are steps
that would usually be part of a larger data flow or control flow
analysis. Specifically, this example collects every basic block in a
function, iterates over them, and counts the number of instructions that
access memory.
