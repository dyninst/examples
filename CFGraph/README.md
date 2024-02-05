# Control Flow Graph

## Create a Graphviz file from the CFG

The following complete example uses ParseAPI to parse a binary and
dump its control flow graph in the Graphviz file format.

---

## Usage

Run the mutator

	$ ./CFGraph /path/to/binary >binary.dot

Convert the DOT file into an image

	$ dot ./binary.dot binary.png

