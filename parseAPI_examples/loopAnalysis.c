#include <iostream>
#include "CodeObject.h"
#include "InstructionDecoder.h"


using namespace std;
using namespace Dyninst;
using namespace ParseAPI;

using namespace InstructionAPI;

void GetLoopInFunc(Function *f) {
	// Get all loops in the function
	vector<Loop*> loops;
	f->getLoops(loops);

	if (loops.size() != 0) cout << "\nDiscovered " << loops.size() << " loops...\n";
	
	// Iterate over all loops
	for (auto lit = loops.begin(); lit != loops.end(); ++lit) {

		Loop *loop = *lit;

		// Get all the entry blocks of the loop
		vector<Block*> entries;
		loop->getLoopEntries(entries);

		// Get all the blocks in the loop
		vector<Block*> blocks;
		loop->getLoopBasicBlocks(blocks);

		// Get all the back edges in the loop
		vector<Edge*> backEdges;
		loop->getBackEdges(backEdges);

		//print results
		auto eit = entries.begin();
		//Block *blk = eit;
		cout << "Found a loop starting at " << *eit << "\n";

		cout << "\tThe loop has " << entries.size() << " entry blocks: ";
		for (;eit != entries.end(); ++eit) cout << *eit << ", ";
		cout << "\n";

		auto bit = blocks.begin();
		cout << "\tThe loop spans " << blocks.size() << " blocks: ";
                for (;bit != blocks.end(); ++bit) cout << *bit << ", ";
		cout << "\n";

		auto bEit = backEdges.begin();
                cout << "\tThe loop has " << backEdges.size() << " loop back points: ";
                for (;bEit != backEdges.end(); ++bEit) cout << *bEit << ", ";
		cout << "\n\n";

	}
}

int main(int argc, char **argv) {
	if(argc != 2){
		printf("Usage: %s <binary path>\n", argv[0]);
		return -1;
	}

	char *binaryPath = argv[1];

	SymtabCodeSource *sts;
    	CodeObject *co;
    	Instruction instr;
    	SymtabAPI::Symtab *symTab;
    	std::string binaryPathStr(binaryPath);
	bool isParsable = SymtabAPI::Symtab::openFile(symTab, binaryPathStr);
    	if(isParsable == false){
		const char *error = "error: file can not be parsed";
		cout << error;
		return - 1;
	}

	sts = new SymtabCodeSource(binaryPath);
	co = new CodeObject(sts);
	//parse the binary given as a command line arg
    	co->parse();

	//get list of all functions in the binary
    	const CodeObject::funclist &all = co->funcs();
    	if(all.size() == 0){
		const char *error = "error: no functions in file";
		cout << error;
		return - 1;
    	}
    	auto fit = all.begin();
    	//Function *f = *fit;
    	//create an Instruction decoder which will convert the binary opcodes to strings
    	//InstructionDecoder decoder(f->isrc()->getPtrToInstruction(f->addr()),
	//	InstructionDecoder::maxInstructionLength,
	//    	f->region()->getArch());

	for(;fit != all.end(); ++fit){
		Function *f = *fit;
		cout << "\n\n\"" << f->name() << "\" :";
		GetLoopInFunc(f);
	}

	cout << "\n\n\"";
}
