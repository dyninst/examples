#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_function.h"
#include "BPatch_image.h"
#include "BPatch_object.h"
#include "BPatch_point.h"
#include "PatchMgr.h"

#include <vector>

namespace dpa = Dyninst::PatchAPI;

// a simple example, just insert a bunch of no ops
class NoopSnippet : public dpa::Snippet {
public:
  bool generate(dpa::Point *pt, Dyninst::Buffer &buffer) override {
    uint8_t byte = 0x90;
    std::cout << "inserting a no op @" << pt << std::endl;
    for (int i = 0; i < 10; i++) {
      buffer.push_back(byte);
    }
    return true;
  }
};

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage:\n\t" << argv[0]
              << " <input binary> <output binary path>\n";
    return -1;
  }

  const char *input_binary = argv[1];
  const char *output_binary = argv[2];

  BPatch bpatch;
  BPatch_binaryEdit *app = bpatch.openBinary(input_binary, false);
  if (!app) {
    std::cerr << "Unable to open '" << input_binary << "'\n";
    return -1;
  }

  BPatch_image *image = app->getImage();
  if (!image) {
    std::cerr << "Failed to get image\n";
    return -1;
  }

  dpa::PatchMgrPtr patchMgr = dpa::convert(image);

  vector<BPatch_object *> objects;
  image->getObjects(objects);

  std::cout << "Found " << objects.size() << " objects\n";

  BPatch_object *batchObj = objects[0];

  // Not mentioned in A.2 of
  // https://dyninst.org/sites/default/files/manuals/dyninst/patchAPI.pdf But
  // found in the header file:  "BPatch_object.h"
  dpa::PatchObject *binobj = dpa::convert(batchObj);

  dpa::Patcher patcher(patchMgr);

  NoopSnippet::Ptr snippet = NoopSnippet::create(new NoopSnippet);

  std::vector<dpa::PatchFunction *> functions;
  binobj->funcs(std::back_inserter(functions));

  for (dpa::PatchFunction *fun : functions) {
    std::vector<dpa::Point *> f_entryPoints;
    patchMgr->findPoints(dpa::Scope(fun), dpa::Point::FuncEntry,
                         back_inserter(f_entryPoints));

    std::cout << fun->name() << " has:\n\t" << f_entryPoints.size()
              << " entry points" << std::endl;

    for (dpa::Point *point : f_entryPoints) {
      std::cerr << "Patching @ " << point << std::endl;
      patcher.add(dpa::PushBackCommand::create(point, snippet));
    }
  }

  patcher.commit();
  app->writeFile(output_binary);

  std::cout << "Done\n";
}
