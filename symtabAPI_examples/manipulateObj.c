#include <stdlib.h>
#include <iostream>
//#include <vector>

#include "CodeObject.h"
#include "Variable.h"
#include "Function.h"
#include "LineInformation.h"
//#include "Symtab.h"
//#include "Function.h"
#include "Symbol.h"
#include "Symtab.h"
#include "symlookup.h"
#include "symutil.h"
//#include "LineInformation.h"
//#include "Variable.h"
//#include "Type.h"
//#include "mapped_module.h"

using namespace Dyninst;
using namespace SymtabAPI;

//Name the object file to be parsed:
std::string file = "libfoo.so";

//Declare a pointer to an object of type Symtab; this represents the file.
Symtab *obj = NULL;

int main(int argc, char** argv) 
{
	if(argc > 2){
		printf("Usage: %s <.so path>\n", argv[0]);
		return -1;
    	}

	if (argc == 2)	file = argv[1];
	else std::cout << "No shared object file supplied, using default object file " << file << "\n\n";




	/*******************    Section 1    ********************/
	// Parse the object file
	bool err = Symtab::openFile(obj, file);

	if (err) {
		std::cout << "Unknown error occured when parsing target object file, quitting...";
		return -1;
	}




	/*******************    Section 2    ********************/
	//Once the object file is parsed successfully and the handle is obtained
	//symbol look up and update operations can be performed in the following way
	std::vector <Symbol *> syms;
	std::vector <Function *> funcs;
	
	// search for a function with demangled (pretty) name “bar”.
	if (obj->findFunctionsByName(funcs, "bar")) {
	       // Add a new (mangled) name to the first function
	       funcs[0]->addMangledName("newname", true);
	}

	// search for symbol of any type with demangled (pretty) name “bar”.
	if (obj->findSymbol(syms, "bar", Symbol::ST_UNKNOWN)) {
		// change the type of the found symbol to type variable(ST_OBJECT)
		syms[0]->setSymbolType(Symbol::ST_OBJECT);
		// These changes are automatically added to symtabAPI; no further
		// actions are required by the user.
	}


	/*******************    Section 3    ********************/
	//create and add new symbol
	
	//obj represents a handle to a parsed object file.
	//Lookup module handle for “DEFAULT_MODULE” 
	//obj->findModuleByName(mod, "DEFAULT_MODULE");

	// Create a new function symbol
	Variable *newVar = obj->createVariable("newIntVar",  // Name of new variable
                                       0x12345,      // Offset from data section
                                       sizeof(int)); // Size of symbol 



	

	/*******************    Section 4    ********************/
	//ymtabAPI gives the ability to query type information
	// create a new struct Type
	// typedef struct{
	//			int field1,
	//			int field2[10]
	// } struct1;
	
	// obj represents a handle to a parsed object file using SymtabAPI
	// Find a handle to the integer type
	Type *lookupType;
	obj->findType(lookupType, "int");

	// Convert the generic type object to the specific scalar type object
	typeScalar *intType = lookupType->getScalarType();

	// container to hold names and types of the new structure type
	dyn_c_vector<std::pair<std::string, Type *>*> fields;

	//create a new array type(int type2[10])
	//		need fix
	std::string arrName = "intArray";
	typeArray *intArray = typeArray::create(arrName,intType,0,9);

	//types of the structure fields
	fields.push_back(new std::pair<std::string, Type *>("field1", intType));
	fields.push_back(new std::pair<std::string, Type *>("field2", intArray));

	//create the structure type
	//		need fix
	std::string structName = "structl";
	typeStruct *struct1 = typeStruct::create(structName, fields, obj);
	



	/*******************    Section 5    ********************/
	//Users can also query line number information
	
	// obj represents a handle to a parsed object file using symtabAPI
	// Container to hold the address range
	//cout << Offset << "\n\n";
	//std::vector< std::pair< Offset, Offset > > ranges;
	std::vector< AddressRange > ranges;

	// Get the address range for the line 30 in source file foo.c
	std::string srcName = "foo.c";
	obj->getAddressRanges( ranges, srcName, 30 );
	


	/*******************    Section 6    ********************/
	//Local variable information can be obtained using symtabAPI

	// Obj represents a handle to a parsed object file using symtabAPI
	// Get the Symbol object representing function bar
	std::vector<Symbol *> bar_syms;
	obj->findSymbol(bar_syms, "bar", Symbol::ST_FUNCTION);

	// Find the local var foo within function bar
	std::vector<localVar*> local_vars;
	bar_syms[0]->getFunction()->findLocalVariable(local_vars, "foo");
	




	return 0;
}
