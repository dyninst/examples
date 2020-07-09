# ParseAPI Examples
This directory contains three examples taken from the parseAPI guidebook, Part 3.

funcDisssemb takes a binary file and outputs the assembly code of all functions in the binary file

loopAnalysis detects all the loops within a binary file and print their locations behaviors to the default output.

traverseCFG prints the control flow graph of a given binary file

Please refer to Makefile and the corresponding .c source codes for compilation and usage.

Please make sure that Dyninst is properly installed, and environment variable```$DYNINST_ROOT ``` exists and points to the root directory of your Dyninst intallation location.


## To complie

```Make <target example name>```
Builds the object file and executable program of the target example.
For example, for funcDisssemb.c, the target example name would be funcDisssemb.

```Make clean```
Wipes out all executables and object files under this directory

# Examples
## funcDisssemb
This example corresponds to ParseAPI example 3.1. The example program takes an input binary and outputs the assembly representation for each function in the target binary.
### example usage
```./funcDisssemb ../InterestingProgram```
### example output
```"_init" :
510: "sub $0x8,%rsp"
514: "mov 0x200acd(%rip),%rax"
51b: "test %rax,%rax"
51e: "jz 0x4(%rip)"
520: "call %rax"
522: "add $0x8,%rsp"

"main" :
560: "push %rbp"
561: "push %rbx"
...```


## traverseCFG
This example corresponds to ParseAPI example 3.2. This example takes a target binary and dumps its control flow graph in the Graphviz file format.
### example usage
./traverseCFG ../InterestingProgram
### example output
```digraph G {
         subgraph cluster_0 {
                 label="_init";
...
	"7c4" -> "ffffffffffffffff" [color=green]

}```


## loopAnalysis
This example corresponds to ParseAPI example 3.3. The example program takes an input binary and discovers all possible loops in all functions.
### example usage
```./loopAnalysis ../InterestingProgram```
### example output
```"_init" :

"__printf_chk" :

"targ550" :

"main" :
Discovered 1 loops...
Found a loop starting at 0x558f8ee60140
        The loop has 1 entry blocks: 0x558f8ee60140,
        The loop spans 4 blocks: 0x558f8ee60140, 0x7fce2c007010, 0x7fce2c008300, 0x7fce2c009560,
        The loop has 1 loop back points: 0x7fce2c009950,

...
```

