# DataflowAPI

## Small code snippets showing ideas of dataflow analysis

These are not standalone programs. They only demonstrate the minimal code necessary
to use a feature of DataflowAPI.

---

## Liveness

Query for live registers.

## Slicing

Perform a backward slice on an indirect jump instruction to determine the instructions that affect the
calculation of the jump target.

## Stack Analysis

Use stack analysis to print out all defined stack heights at the first instruction in a block.

## Symbolic Evaluation

Expand a slice to ASTs and analyze them.
