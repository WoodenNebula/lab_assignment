#include "src/commons.hpp"

#include <cmath>
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

bool isNumber(const std::string& token) {
    if (token.empty()) return false;
    std::size_t pos = 0;
    if (token[pos] == '+' || token[pos] == '-') ++pos;
    bool seenDot = false;
    for (; pos < token.size(); ++pos) {
        if (token[pos] == '.') {
            if (seenDot) return false;
            seenDot = true;
        }
        else if (!std::isdigit(static_cast<unsigned char>(token[pos]))) {
            return false;
        }
    }
    return pos > 0;
}

void printQuads(const std::vector<Quadruple>& quads) {
    for (size_t i = 0; i < quads.size(); ++i) {
        const auto& q = quads[i];
        Surab::Log("{}: {} {} {} -> {}", i + 1, q.op, q.arg1, q.arg2, q.result);
    }
}

static bool TryGetConstantValue(const std::unordered_map<std::string, double>& constTable,
    const std::string& token, double& value) {
    if (isNumber(token)) {
        value = std::stod(token);
        return true;
    }

    const auto it = constTable.find(token);
    if (it != constTable.end()) {
        value = it->second;
        return true;
    }

    return false;
}

static void PropagateAndFold(std::vector<Quadruple>& quadList, std::unordered_map<std::string, double>& constTable) {
    bool modified = true;
    while (modified) {
        modified = false;

        for (auto& quad : quadList) {
            // propagate known constants into operands
            if (!quad.arg1.empty()) {
                double val = 0.0;
                if (TryGetConstantValue(constTable, quad.arg1, val) && quad.arg1 != std::to_string(val)) {
                    quad.arg1 = std::to_string(val);
                    modified = true;
                }
            }
            if (!quad.arg2.empty()) {
                double val = 0.0;
                if (TryGetConstantValue(constTable, quad.arg2, val) && quad.arg2 != std::to_string(val)) {
                    quad.arg2 = std::to_string(val);
                    modified = true;
                }
            }

            // folding for constant arithmetic
            if (quad.op != "=" && quad.op != "goto" && quad.op != "ifFalse" && quad.op != "label") {
                double lhs = 0.0;
                double rhs = 0.0;
                if (TryGetConstantValue(constTable, quad.arg1, lhs) && TryGetConstantValue(constTable, quad.arg2, rhs)) {
                    double value = 0.0;
                    if (quad.op == "+") value = lhs + rhs;
                    else if (quad.op == "-") value = lhs - rhs;
                    else if (quad.op == "*") value = lhs * rhs;
                    else if (quad.op == "/" && rhs != 0.0) value = lhs / rhs;
                    else continue;

                    const std::string newValue = std::to_string(value);
                    if (quad.op != "=" || quad.arg1 != newValue || !quad.arg2.empty()) {
                        quad.op = "=";
                        quad.arg1 = newValue;
                        quad.arg2 = "";
                        constTable[quad.result] = value;
                        modified = true;
                    }
                }
            }

            // assignment tracking: constant assignment updates constTable; non-constant assignment invalidates it
            if (quad.op == "=") {
                double value = 0.0;
                if (TryGetConstantValue(constTable, quad.arg1, value)) {
                    if (!constTable.count(quad.result) || std::abs(constTable[quad.result] - value) > 1e-9) {
                        constTable[quad.result] = value;
                        modified = true;
                    }
                }
                else if (quad.arg1 != "" && !isNumber(quad.arg1) && constTable.count(quad.result)) {
                    constTable.erase(quad.result);
                    modified = true;
                }
            }
        }
    }
}

int main() {
    Header("Constant Folding and Propagation");

    std::vector<Quadruple> quads = {
        { "=", "4", "", "t1" },
        { "*", "t1", "5", "t2" },
        { "+", "x", "y", "t3" },
        { "=", "t2", "", "result" },
        { "=", "t3", "", "x" },
        { "+", "x", "2", "t4" }
    };

    std::unordered_map<std::string, double> constTable;

    Surab::Log("Original quadruples:");
    printQuads(quads);

    PropagateAndFold(quads, constTable);

    Surab::Log("Constant table:");
    for (const auto& [name, value] : constTable) {
        Surab::Log("{} = {}", name, value);
    }

    Surab::Log("Optimized quadruples:");
    printQuads(quads);

    Footer();
    return 0;
}
