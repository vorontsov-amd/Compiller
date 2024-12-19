#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

extern "C" void simInit();
extern "C" void simExit();
extern "C" void simFlush();
extern "C" void simClearWindow();
extern "C" void simPutPixel(int x, int y, int argb);

using namespace llvm;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ll>" << std::endl;
        return 1;
    }

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    LLVMContext context;
    SMDiagnostic error;

    std::unique_ptr<Module> module = parseIRFile(argv[1], error, context);
    if (!module) {
        error.print(argv[0], errs());
        return 1;
    }

    std::string errStr;
    ExecutionEngine* engine = EngineBuilder(std::move(module))
                                  .setErrorStr(&errStr)
                                  .setMCJITMemoryManager(std::make_unique<SectionMemoryManager>())
                                  .create();

    if (!engine) {
        std::cerr << "Failed to create ExecutionEngine: " << errStr << std::endl;
        return 1;
    }

    std::cout << "Create ExecutionEngine\n";
    engine->addGlobalMapping("simInit", reinterpret_cast<uint64_t>(&simInit));
    engine->addGlobalMapping("simExit", reinterpret_cast<uint64_t>(&simExit));
    engine->addGlobalMapping("simFlush", reinterpret_cast<uint64_t>(&simFlush));
    engine->addGlobalMapping("simClearWindow", reinterpret_cast<uint64_t>(&simClearWindow));
    engine->addGlobalMapping("simPutPixel", reinterpret_cast<uint64_t>(&simPutPixel));

    Function* mainFunction = engine->FindFunctionNamed("main");
    if (!mainFunction) {
        std::cerr << "Function 'main' not found in the IR file." << std::endl;
        return 1;
    }

    std::vector<GenericValue> args;
    std::cout << "Run main\n";
    GenericValue result = engine->runFunction(mainFunction, args);
    llvm::outs() << "Result from 'main': " << result.IntVal << "\n";
    return 0;
}
