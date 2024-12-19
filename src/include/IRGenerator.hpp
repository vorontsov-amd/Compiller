#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

struct IRGenerator final {
    std::unordered_map<std::string, llvm::Value*> local_vars;
    llvm::LLVMContext context;
    llvm::Module module {"Module", context};
    llvm::IRBuilder<> builder {context};
    llvm::Function* current_function = nullptr;
    std::unordered_map<std::string, llvm::Value*> string_literals;
    std::unordered_map<double, llvm::Value*> fp_literals;

    void dump(const std::string& filename) {
        std::error_code EC;
        llvm::raw_fd_ostream file(filename, EC);
        if (EC) {
            llvm::errs() << "Error opening file: " << EC.message() << "\n";
            return;
        }
        module.print(file, nullptr);
        file.close();
    }
};