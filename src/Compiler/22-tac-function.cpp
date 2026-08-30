#include "src/commons.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

struct FunctionInfo {
    std::string name;
    std::vector<std::string> params;
};

std::unordered_map<std::string, FunctionInfo> functionTable;
std::vector<Quadruple> quadList;
int tempCount = 1;

std::string NewTemp() {
    return "t" + std::to_string(tempCount++);
}

void AddFunction(const std::string& name, const std::vector<std::string>& params) {
    functionTable[name] = { name, params };
}

bool VerifyCall(const std::string& name, int argCount) {
    auto it = functionTable.find(name);
    if (it == functionTable.end()) {
        Surab::LogError("Function '{}' is not declared.", name);
        return false;
    }

    if (static_cast<int>(it->second.params.size()) != argCount) {
        Surab::LogError("Function '{}' expects {} args but got {}.", name, it->second.params.size(), argCount);
        return false;
    }

    return true;
}

void Emit(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result) {
    quadList.push_back({ op, arg1, arg2, result });
}

void PrintQuadruples() {
    for (size_t i = 0; i < quadList.size(); ++i) {
        const auto& q = quadList[i];
        Surab::Log("{}: {} {} {} -> {}", i + 1, q.op, q.arg1, q.arg2, q.result);
    }
}

int main() {
    Header("TAC for Functions and Parameters");

    AddFunction("foo", { "a", "b" });

    Emit("FUNC_BEGIN", "foo", "", "");
    Emit("PARAM", "a", "", "");
    Emit("PARAM", "b", "", "");
    Emit("RETURN", "a + b", "", "");
    Emit("FUNC_END", "foo", "", "");

    const std::string callResult = NewTemp();
    Emit("PARAM", "x", "", "");
    Emit("PARAM", "y", "", "");
    Emit("CALL", "foo", "2", callResult);
    Emit("ASSIGN", callResult, "", "z");

    Surab::Log("Call verification for foo(2 args): {}", VerifyCall("foo", 2) ? "valid" : "invalid");
    Surab::Log("Call verification for foo(3 args): {}", VerifyCall("foo", 3) ? "valid" : "invalid");
    PrintQuadruples();

    Footer();
    return 0;
}
