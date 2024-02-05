# unstrip

## A library fingerprinting tool

The tool identifies library wrapper functions in an input binary and outputs
a new binary with meaningful names assigned to these functions.

To see usage information, run unstrip without an arguments.

% ./unstrip
Usage: ./unstrip
		-f <binary>
		-o <output file> [optional]
                -l [optional: learning mode]

Provide the program with a stripped binary and the destination binary.

% ./unstrip -f bin-stripped -o bin

Or, invoke unstrip in learning mode to add new descriptors to the database.
Currently, this mode requires that a static binary that includes libray code
from which patterns should be generated. To generate ths binary, run:

% ./generate-learn-binary.bash <library>

The binary will be placed in learning_libraries and can be used by unstrip as follows:

% ./unstrip -f learning-binaries/lib-binary -l
