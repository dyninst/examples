#include "BPatch.h"
#include "BPatch_function.h"
#include "BPatch_object.h"
#include "BPatch_image.h"
#include "BPatch_point.h"
#include "BPatch_snippet.h"

#include "PatchMgr.h"
#include "PatchModifier.h"
#include "Point.h"
#include "Snippet.h"

#include "Instruction.h"
#include "InstructionDecoder.h"
#include "Register.h"
#include "Expression.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>

using namespace Dyninst;
using namespace Dyninst::PatchAPI;
using namespace InstructionAPI;

#define TOSTRING(x) #x

// BPatch is the top level object in Dyninst
BPatch bpatch;

struct instruction_t {
    unsigned long address;
    std::vector<unsigned char> bytes;
};

std::vector<instruction_t> tracked_instructions;

class AsmSnippet : public Dyninst::PatchAPI::Snippet {
    public:
        AsmSnippet(std::vector<unsigned char> bytes) {instruction_bytes = bytes;}
        bool generate(Dyninst::PatchAPI::Point* pt, Dyninst::Buffer& buf) override {
            std::cerr << "INSERT: 0x" << std::hex << pt->addr() << " format = " << pt->insn().format() << std::endl;
            buf.copy(instruction_bytes.data(), instruction_bytes.size());
            return true;
        }

    private:
        std::vector<unsigned char> instruction_bytes;
};

void parse_file(std::string file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Could not open " << file_path << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string address_str, byte_str;
        iss >> address_str;

        instruction_t instruction;
        instruction.address = std::stoul(address_str, nullptr, 16);

        while (iss >> std::hex >> byte_str) {
            if (byte_str.length() == 2) {
                unsigned int byte = std::stoul(byte_str, nullptr, 16);
                instruction.bytes.push_back(static_cast<unsigned char>(byte));
            }
        }

        tracked_instructions.push_back(instruction);
    }

    file.close();
}

BPatch_point* FindPoint(BPatch_image* image, Dyninst::Address address) {
    std::vector<BPatch_point*> points;
    image->findPoints(address, points);

    if (points.empty()) {
        std::cerr << "No point found at address 0x" << std::hex << address << std::endl;
        return nullptr;
    }

    if (points.size() > 1) {
        std::cerr << "Warning: Multiple points found at address 0x" << std::hex << address << std::endl;
    }

    return points.front();
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_binary> <output_binary> <list>" << std::endl;
        return -1;
    }

#ifdef __ARCH_32__
    setenv("DYNINSTAPI_RT_LIB", DYNINSTAPI_RT_LIB, 1);
#endif

#ifdef __ARCH_64__
    setenv("DYNINSTAPI_RT_LIB", DYNINSTAPI_RT_LIB, 1);
#endif

    parse_file(argv[3]);

    BPatch_binaryEdit* app = bpatch.openBinary(argv[1]);
    if (!app) {
        std::cerr << "Failed to open binary" << std::endl;
        return -1;
    }

    BPatch_image* image = app->getImage();

    for (auto& instruction : tracked_instructions) {

        std::vector<BPatch_point*> points;
        
        auto point = FindPoint(image, instruction.address);

        Snippet::Ptr codeBufferSnippet = AsmSnippet::create(new AsmSnippet(instruction.bytes));
        if(!codeBufferSnippet) std::cerr << "AsmSnippet::create failed" << std::endl;

        Point* patch_point = Dyninst::PatchAPI::convert(point, BPatch_callAfter);
        if(!patch_point) std::cerr << "Dyninst::PatchAPI::convert failed" << std::endl;

        if (!patch_point->pushBack(codeBufferSnippet))
            std::cerr << "Failed to push back snippet at address: 0x" << std::hex << instruction.address << std::endl;
    }

    if (!app->writeFile(argv[2])) {
        std::cerr << "Failed to write the instrumented binary to " << argv[2] << std::endl;
        return -1;
    }

    return 0;
}
