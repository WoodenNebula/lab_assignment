#include "src/commons.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

struct AssemblyInstruction {
    std::string label;
    std::string mnemonic;
    std::string operand1;
    std::string operand2;
    std::string comment;

    std::string ToString() const {
        std::string out;
        if (!label.empty()) out += label + ":";
        if (!mnemonic.empty()) {
            out += " " + mnemonic;
            if (!operand1.empty()) {
                out += " " + operand1;
                if (!operand2.empty()) out += ", " + operand2;
            }
        }
        if (!comment.empty()) out += " ; " + comment;
        return out;
    }
};

class AsmGenerator {
public:
    AsmGenerator() {
        registerOrder = { "AX", "BX", "CX", "DX" };
    }

    std::vector<AssemblyInstruction> Generate(const std::vector<Quadruple>& quads) {
        generated.clear();
        offsets.clear();
        nextOffset = 2;
        regOwners.clear();
        regAssignments.clear();

        for (const auto& q : quads) {
            if (q.result != "" && q.result.find("L") != 0 && q.result.find("label") != 0) {
                GetOffset(q.result);
            }
            if (!q.arg1.empty() && q.arg1.find("L") != 0 && q.arg1.compare("0") != 0 && q.arg1.find("label") != 0) {
                GetOffset(q.arg1);
            }
            if (!q.arg2.empty() && q.arg2.find("L") != 0 && q.arg2.compare("0") != 0 && q.arg2.find("label") != 0) {
                GetOffset(q.arg2);
            }
        }

        int localSize = nextOffset - 2;
        if (localSize < 0) localSize = 0;

        Emit("", "PUSH", "BP", "", "save old base pointer");
        Emit("", "MOV", "BP", "SP", "set new base pointer");
        Emit("", "SUB", "SP", std::to_string(localSize), "allocate locals");

        for (const auto& q : quads) {
            if (q.op == "=") {
                EmitAssign(q.result, q.arg1);
            }
            else if (q.op == "+" || q.op == "-" || q.op == "*" || q.op == "/") {
                EmitBinary(q.op, q.arg1, q.arg2, q.result);
            }
            else if (q.op == "==" || q.op == "!=" || q.op == ">" || q.op == ">=" || q.op == "<" || q.op == "<=") {
                EmitCompare(q.op, q.arg1, q.arg2, q.result);
            }
            else if (q.op == "goto") {
                Emit("", "JMP", q.result, "", "unconditional jump");
            }
            else if (q.op == "if_false") {
                EmitConditionalFalse(q.arg1, q.arg2, q.result);
            }
            else if (q.op == "if") {
                EmitConditionalTrue(q.arg1, q.arg2, q.result);
            }
            else if (q.op == "return") {
                EmitReturn(q.arg1);
            }
            else if (q.op == "label") {
                EmitLabel(q.result);
            }
            else {
                Emit("", ";", "", "", "unsupported op: " + q.op);
            }
        }

        Emit("", "MOV", "SP", "BP", "restore stack pointer");
        Emit("", "POP", "BP", "", "restore base pointer");
        Emit("", "RET", "", "", "return to caller");

        return generated;
    }

    std::string MemoryOperand(const std::string& symbol) {
        if (IsImmediate(symbol)) return symbol;
        if (IsRegister(symbol)) return symbol;

        int offset = GetOffset(symbol);
        return "[BP - " + std::to_string(offset) + "]";
    }

    std::string OperandForLoad(const std::string& symbol) {
        if (IsImmediate(symbol)) return symbol;
        if (IsRegister(symbol)) return symbol;
        return MemoryOperand(symbol);
    }

    void EmitLabel(const std::string& label) {
        generated.push_back({ label, "", "", "", "" });
    }

    void Emit(const std::string& label, const std::string& mnemonic,
        const std::string& operand1, const std::string& operand2,
        const std::string& comment = "") {
        generated.push_back({ label, mnemonic, operand1, operand2, std::format("{}{}{}", GREY, comment, RESET) });
    }

private:
    std::vector<AssemblyInstruction> generated;
    std::unordered_map<std::string, int> offsets;
    std::unordered_map<std::string, std::string> regOwners;
    std::unordered_map<std::string, std::string> regAssignments;
    std::vector<std::string> registerOrder;
    int nextOffset = 2;

    int GetOffset(const std::string& symbol) {
        if (symbol.empty()) return 0;

        if (IsImmediate(symbol) || IsRegister(symbol) || symbol.rfind("L", 0) == 0) {
            return 0;
        }

        if (offsets.find(symbol) == offsets.end()) {
            offsets[symbol] = nextOffset;
            nextOffset += 2;
        }
        return offsets[symbol];
    }

    bool IsRegister(const std::string& value) const {
        static const std::set<std::string> regs = { "AX", "BX", "CX", "DX" };
        return regs.count(value) > 0;
    }

    bool IsImmediate(const std::string& value) const {
        if (value.empty()) return false;
        if (value == "0") return true;
        if (value[0] == '-' || value[0] == '+') {
            return value.size() > 1 && value.find_first_not_of("-+0123456789") == std::string::npos;
        }
        return value.find_first_not_of("0123456789") == std::string::npos;
    }

    std::string AllocateRegister(const std::string& symbol) {
        if (regAssignments.find(symbol) != regAssignments.end()) {
            return regAssignments[symbol];
        }

        for (const std::string& reg : registerOrder) {
            if (regOwners.find(reg) == regOwners.end()) {
                regOwners[reg] = symbol;
                regAssignments[symbol] = reg;
                return reg;
            }
        }

        std::string spilledReg = registerOrder.front();
        std::string spilledSymbol = regOwners[spilledReg];

        Emit("", "PUSH", spilledReg, "", "spill register to stack");
        Emit("", "MOV", MemoryOperand(spilledSymbol), spilledReg, "save spilled value");

        regOwners.erase(spilledReg);
        regOwners[spilledReg] = symbol;
        regAssignments[symbol] = spilledReg;
        regAssignments.erase(spilledSymbol);
        return spilledReg;
    }

    void ReleaseRegister(const std::string& symbol) {
        auto it = regAssignments.find(symbol);
        if (it == regAssignments.end()) return;
        std::string reg = it->second;
        regAssignments.erase(it);
        regOwners.erase(reg);
    }

    void EmitAssign(const std::string& dst, const std::string& src) {
        std::string srcVal = OperandForLoad(src);
        if (IsImmediate(src) || src.empty()) {
            Emit("", "MOV", "AX", srcVal, "load immediate/value");
        }
        else {
            Emit("", "MOV", "AX", srcVal, "load source");
        }
        Emit("", "MOV", MemoryOperand(dst), "AX", "store into destination");
    }

    void EmitBinary(const std::string& op, const std::string& left, const std::string& right, const std::string& dst) {
        std::string reg = AllocateRegister(dst);
        Emit("", "MOV", reg, OperandForLoad(left), "load left operand");

        if (op == "+") {
            Emit("", "ADD", reg, OperandForLoad(right), "add right operand");
        }
        else if (op == "-") {
            Emit("", "SUB", reg, OperandForLoad(right), "subtract right operand");
        }
        else if (op == "*") {
            Emit("", "MOV", "AX", OperandForLoad(left), "multiply left operand");
            Emit("", "IMUL", "WORD PTR " + OperandForLoad(right), "", "multiply by right operand");
            Emit("", "MOV", reg, "AX", "move product into temp");
        }
        else if (op == "/") {
            Emit("", "MOV", "AX", OperandForLoad(left), "divide left operand");
            Emit("", "MOV", "DX", "0", "prepare unsigned division");
            Emit("", "DIV", "WORD PTR " + OperandForLoad(right), "", "divide by right operand");
            Emit("", "MOV", reg, "AX", "store quotient");
        }

        Emit("", "MOV", MemoryOperand(dst), reg, "store result");
        ReleaseRegister(dst);
    }

    void EmitCompare(const std::string& op, const std::string& left, const std::string& right, const std::string& label) {
        Emit("", "MOV", "AX", OperandForLoad(left), "compare left operand");
        Emit("", "CMP", "AX", OperandForLoad(right), "compare with right operand");

        if (op == "==") Emit("", "JE", label, "", "jump if equal");
        else if (op == "!=") Emit("", "JNE", label, "", "jump if not equal");
        else if (op == ">") Emit("", "JG", label, "", "jump if greater");
        else if (op == ">=") Emit("", "JGE", label, "", "jump if greater or equal");
        else if (op == "<") Emit("", "JL", label, "", "jump if less");
        else if (op == "<=") Emit("", "JLE", label, "", "jump if less or equal");
    }

    void EmitConditionalFalse(const std::string& cond, const std::string& value, const std::string& label) {
        Emit("", "MOV", "AX", OperandForLoad(cond), "load condition");
        Emit("", "CMP", "AX", OperandForLoad(value), "compare against value");
        Emit("", "JE", label, "", "jump if condition is false");
    }

    void EmitConditionalTrue(const std::string& cond, const std::string& value, const std::string& label) {
        Emit("", "MOV", "AX", OperandForLoad(cond), "load condition");
        Emit("", "CMP", "AX", OperandForLoad(value), "compare against value");
        Emit("", "JNE", label, "", "jump if condition is true");
    }

    void EmitReturn(const std::string& value) {
        if (!value.empty()) {
            Emit("", "MOV", "AX", OperandForLoad(value), "move return value into AX");
        }
        Emit("", "MOV", "SP", "BP", "restore stack pointer");
        Emit("", "POP", "BP", "", "restore base pointer");
        Emit("", "RET", "", "", "return");
    }
};

std::string FormatQuadruples(const std::vector<Quadruple>& quads) {
    std::ostringstream oss;
    oss << "Quadruples:\n";
    for (size_t i = 0; i < quads.size(); ++i) {
        const auto& q = quads[i];
        oss << "  [" << i << "] (" << q.op << ", " << q.arg1 << ", " << q.arg2 << ", " << q.result << ")\n";
    }
    return oss.str();
}

int main() {
    Header("8086 Code Generation");

    std::vector<Quadruple> quads = {
        { "=", "a", "", "t1" },
        { "+", "t1", "b", "t2" },
        { "*", "t2", "c", "t3" },
        { "if_false", "t3", "0", "L1" },
        { "=", "0", "", "result" },
        { "goto", "", "", "L2" },
        { "label", "", "", "L1" },
        { "=", "1", "", "result" },
        { "label", "", "", "L2" },
        { "return", "result", "", "" }
    };

    AsmGenerator generator;

    Surab::Log("{}", FormatQuadruples(quads));

    auto asmCode = generator.Generate(quads);

    Surab::Log("Generated 8086 MASM/TASM assembly:");
    for (const auto& line : asmCode) {
        if (line.label.empty() && line.mnemonic.empty()) {
            continue;
        }
        Surab::Log("{}", line.ToString());
    }

    Footer();
    return 0;
}
