# PatchAPI

## Demonstrations of using the ParseAPI toolkit

---

## Address Space

An address space plugin to manage memory.

## CFG Maker

A custom function CFG generator to trace when a function is parsed.

## CFG Traversal

Traverses the control flow graph (CFG) of a binary, printing the names of the functions
and addresses of the basic blocks encountered.

## Finding Points

Find the entry points for every function.

## Nop Patching

Similar to Insert Snippet, insert a sequence of x86 nop instructions into the beginning of every function in a binary.
This creates a "nop slide" that is sometimes seen in adversarial binaries.

## Patch Modification

PatchAPI provides binary modification with the `PatchModifier` class
to manipulate the CFG. The three key benefits of the PatchAPI modification interface are
abstraction, safety, and interactivity. We use the CFG as a mechanism
for transforming binaries in a platform-independent way that requires no
instruction-level knowledge by the user. These transformations are
limited to ensure that the CFG can always be used to instantiate code,
and thus the user can avoid unintended side-effects of modification.
Finally, modifications to the CFG are represented in that CFG, allowing
users to iteratively combine multiple CFG transformations to achieve
their goals.

## Point Creation

A custom point generator to trace when an instrumentation point is created.
