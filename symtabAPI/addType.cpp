#include "Symtab.h"
#include "Type.h"
#include "concurrent.h"

#include <iostream>
#include <string>

namespace st = Dyninst::SymtabAPI;

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " file\n";
    return -1;
  }

  auto file_name = argv[1];

  st::Symtab* obj{};

  if(!st::Symtab::openFile(obj, file_name)) {
    std::cerr << "Unable to open " << file_name << '\n';
    return -1;
  }

  /** Create a new struct type
   *
   *   struct struct1 {
   *     int field1,
   *     int field2[10]
   *   };
   */

  // Find a handle to the integer type
  st::Type* lookupType;
  obj->findType(lookupType, "int");

  // Convert to a scalar
  auto* intType = lookupType->getScalarType();

  // create a new array type for 'field2'
  std::string iarr_name{"intArray"};
  auto* intArray = st::typeArray::create(iarr_name, intType, 0, 9, obj);

  // container to hold names and types of the new structure type
  using field_t = std::pair<std::string, st::Type*>;
  Dyninst::dyn_c_vector<field_t*> fields;
  auto f1 = field_t{"field1", intType};
  auto f2 = field_t{"field2", intArray};

  fields.emplace_back(&f1);
  fields.emplace_back(&f2);

  // create the structure type
  std::string struct_name{"struct1"};
  auto* struct1 = st::typeStruct::create(struct_name, fields, obj);
  obj->addType(struct1);
}
