#include "src/commons.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string Trim(const std::string& s) {
    std::string out = s;
    while (!out.empty() && std::isspace(static_cast<unsigned char>(out.front()))) out.erase(out.begin());
    while (!out.empty() && std::isspace(static_cast<unsigned char>(out.back()))) out.pop_back();
    return out;
}

std::string NormalizeOperand(const std::string& s) {
    std::string out;
    for (char ch : s) {
        if (ch == ',' || std::isspace(static_cast<unsigned char>(ch))) continue;
        out += static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return out;
}

std::string StripComment(const std::string& s) {
    auto pos = s.find(';');
    if (pos == std::string::npos) return Trim(s);
    return Trim(s.substr(0, pos));
}

bool ParseLabel(const std::string& instr, std::string& label) {
    std::string text = StripComment(instr);
    if (text.empty() || text.back() != ':') return false;
    label = NormalizeOperand(text.substr(0, text.size() - 1));
    return true;
}

bool ParseTwoOperand(const std::string& instr, std::string& op, std::string& left, std::string& right) {
    std::string text = StripComment(instr);
    if (text.empty()) return false;

    std::stringstream ss(text);
    std::string token;
    ss >> token;
    if (!ss) return false;

    op = NormalizeOperand(token);
    std::string rest;
    std::getline(ss, rest);
    rest = Trim(rest);
    if (rest.empty()) return false;

    auto comma = rest.find(',');
    if (comma == std::string::npos) return false;

    left = NormalizeOperand(Trim(rest.substr(0, comma)));
    right = NormalizeOperand(Trim(rest.substr(comma + 1)));
    return true;
}

bool ParseMov(const std::string& instr, std::string& dest, std::string& src) {
    std::string op, left, right;
    if (!ParseTwoOperand(instr, op, left, right)) return false;
    if (op != "MOV") return false;
    dest = left;
    src = right;
    return true;
}

bool ParseBinaryOp(const std::string& instr, std::string& op, std::string& dest, std::string& operand) {
    std::string left, right;
    if (!ParseTwoOperand(instr, op, left, right)) return false;
    if (op != "ADD" && op != "SUB" && op != "MUL" && op != "IMUL" && op != "SHL" && op != "DIV") return false;
    dest = left;
    operand = right;
    return true;
}

bool ParseJump(const std::string& instr, std::string& target) {
    std::string text = StripComment(instr);
    if (text.rfind("JMP ", 0) == 0) {
        target = NormalizeOperand(Trim(text.substr(4)));
        return true;
    }
    return false;
}

bool IsZero(const std::string& value) {
    return NormalizeOperand(value) == "0";
}

bool IsOne(const std::string& value) {
    return NormalizeOperand(value) == "1";
}

bool IsTwo(const std::string& value) {
    return NormalizeOperand(value) == "2";
}

void EraseRange(std::vector<std::string>& instrs, size_t start, size_t end) {
    if (start >= end || end > instrs.size()) return;
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(start), instrs.begin() + static_cast<std::ptrdiff_t>(end));
}

void applyPeephole(std::vector<std::string>& instrs) {
    bool changed = true;
    while (changed) {
        changed = false;

        for (size_t i = 0; i + 1 < instrs.size(); ++i) {
            std::string dest1, src1, dest2, src2;
            if (ParseMov(instrs[i], dest1, src1) && ParseMov(instrs[i + 1], dest2, src2)) {
                if (NormalizeOperand(dest1) == NormalizeOperand(src2) && NormalizeOperand(src1) == NormalizeOperand(dest2)) {
                    EraseRange(instrs, i + 1, i + 2);
                    changed = true;
                    break;
                }
            }

            std::string op1, left1, right1;
            if (ParseBinaryOp(instrs[i], op1, left1, right1)) {
                if ((op1 == "ADD" || op1 == "SUB") && IsZero(right1)) {
                    EraseRange(instrs, i, i + 1);
                    changed = true;
                    break;
                }
                if (op1 == "MUL" && IsZero(right1)) {
                    instrs[i] = "MOV " + left1 + ", 0";
                    changed = true;
                    break;
                }
                if (op1 == "MUL" && IsOne(right1)) {
                    EraseRange(instrs, i, i + 1);
                    changed = true;
                    break;
                }
                if (op1 == "IMUL" && IsTwo(right1)) {
                    instrs[i] = "SHL " + left1 + ", 1";
                    changed = true;
                    break;
                }
            }

            std::string jumpTarget;
            if (ParseJump(instrs[i], jumpTarget) && i + 1 < instrs.size()) {
                std::string labelName;
                if (ParseLabel(instrs[i + 1], labelName) && NormalizeOperand(labelName) == NormalizeOperand(jumpTarget)) {
                    EraseRange(instrs, i, i + 1);
                    changed = true;
                    break;
                }
            }

            if (ParseJump(instrs[i], jumpTarget)) {
                size_t next = i + 1;
                while (next < instrs.size()) {
                    std::string labelName;
                    if (ParseLabel(instrs[next], labelName)) break;
                    ++next;
                }
                if (next > i + 1) {
                    EraseRange(instrs, i + 1, next);
                    changed = true;
                    break;
                }
            }
        }
    }
}

void printInstructions(const std::vector<std::string>& instrs) {
    for (size_t i = 0; i < instrs.size(); ++i) {
        Surab::Log("{}: {}", i + 1, instrs[i]);
    }
}

int main() {
    Header("Peephole Optimization");

    std::vector<std::string> instrs = {
        "MOV AX, [BP-2]",
        "MOV [BP-2], AX",
        "ADD AX, 0",
        "SUB AX, 0",
        "MUL AX, 1",
        "MUL AX, 0",
        "IMUL AX, 2",
        "JMP L1",
        "L1:",
        "MOV AX, [BP-4]",
        "NOP",
        "JMP L2",
        "L2:",
        "MOV AX, [BP-8]"
    };

    Surab::Log("Original assembly:");
    printInstructions(instrs);

    applyPeephole(instrs);

    Surab::Log("Optimized assembly:");
    printInstructions(instrs);

    Footer();
    return 0;
}
