#include "src/commons.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

int labelCount = 1;

std::string NewLabel() {
    return "L" + std::to_string(labelCount++);
}

std::vector<int> MakeList(int idx) {
    return { idx };
}

std::vector<int> MergeLists(const std::vector<int>& left, const std::vector<int>& right) {
    std::vector<int> merged = left;
    merged.insert(merged.end(), right.begin(), right.end());
    return merged;
}

void Backpatch(std::vector<Quadruple>& quadList, const std::vector<int>& list, const std::string& label) {
    for (int idx : list) {
        if (idx >= 0 && idx < static_cast<int>(quadList.size())) {
            quadList[idx].result = label;
        }
    }
}

void Emit(std::vector<Quadruple>& quadList, const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result) {
    quadList.push_back({ op, arg1, arg2, result });
}

void PrintQuadruples(const std::vector<Quadruple>& quads) {
    for (size_t i = 0; i < quads.size(); ++i) {
        const auto& q = quads[i];
        Surab::Log("{}: {} {}, {}, {}", i + 1, q.op, q.arg1, q.arg2, q.result);
    }
}

void EmitShortCircuitLogical(std::vector<Quadruple>& quadList,
    const std::string& left,
    const std::string& right,
    const std::string& op,
    const std::string& trueLabel,
    const std::string& falseLabel) {

    if (op == "&&") {
        // left must be true before checking right
        const int leftFalseIdx = static_cast<int>(quadList.size());
        Emit(quadList, "ifFalse", left, "", "");
        std::vector<int> falseList = MakeList(leftFalseIdx);

        const int rightFalseIdx = static_cast<int>(quadList.size());
        Emit(quadList, "ifFalse", right, "", "");
        falseList = MergeLists(falseList, MakeList(rightFalseIdx));

        Backpatch(quadList, falseList, falseLabel);
        Emit(quadList, "goto", "", "", trueLabel);
    }
    else if (op == "||") {
        // left must be false before checking right
        const int leftFalseIdx = static_cast<int>(quadList.size());
        Emit(quadList, "ifFalse", left, "", "");
        std::vector<int> trueList = MakeList(leftFalseIdx);

        const int rightFalseIdx = static_cast<int>(quadList.size());
        Emit(quadList, "ifFalse", right, "", "");
        trueList = MergeLists(trueList, MakeList(rightFalseIdx));

        Backpatch(quadList, trueList, trueLabel);
        Emit(quadList, "goto", "", "", falseLabel);
    }
}

int main() {
    Header("TAC for Control Flow");

    std::vector<Quadruple> quadList;
    const std::string L_then = NewLabel();
    const std::string L_else = NewLabel();
    const std::string L_exit = NewLabel();

    // if (flag) result = 1 else result = 0
    int condIndex = static_cast<int>(quadList.size());
    Emit(quadList, "ifFalse", "flag", "", "");
    std::vector<int> falseList = MakeList(condIndex);

    Emit(quadList, "label", "", "", L_then);
    Emit(quadList, "assign", "result", "", "1");
    Emit(quadList, "goto", "", "", L_exit);

    Backpatch(quadList, falseList, L_else);
    Emit(quadList, "label", "", "", L_else);
    Emit(quadList, "assign", "result", "", "0");
    Emit(quadList, "label", "", "", L_exit);

    Surab::Log("{:=^10}", "");
    Surab::Log("if (flag) result = 1 else result = 0");
    PrintQuadruples(quadList);
    Surab::Log("{:=^10}", "");

    std::vector<Quadruple> loopQuads;
    const std::string L_begin = NewLabel();
    const std::string L_body = NewLabel();
    const std::string L_done = NewLabel();

    // while (i < 5) sum = sum + i
    Emit(loopQuads, "label", "", "", L_begin);
    int loopCondIndex = static_cast<int>(loopQuads.size());
    Emit(loopQuads, "ifFalse", "i < 5", "", "");
    std::vector<int> loopFalseList = MakeList(loopCondIndex);

    Emit(loopQuads, "label", "", "", L_body);
    Emit(loopQuads, "assign", "sum", "", "sum + i");
    Emit(loopQuads, "goto", "", "", L_begin);

    Backpatch(loopQuads, loopFalseList, L_done);
    Emit(loopQuads, "label", "", "", L_done);

    Surab::Log("{:=^10}", "");
    Surab::Log("while (i < 5) sum = sum + i");
    PrintQuadruples(loopQuads);
    Surab::Log("{:=^10}", "");

    std::vector<Quadruple> logicalQuads;
    const std::string L_true = NewLabel();
    const std::string L_false = NewLabel();
    EmitShortCircuitLogical(logicalQuads, "a", "b", "&&", L_true, L_false);
    Emit(logicalQuads, "label", "", "", L_true);
    Emit(logicalQuads, "assign", "x", "", "1");
    Emit(logicalQuads, "goto", "", "", L_done);
    Emit(logicalQuads, "label", "", "", L_false);
    Emit(logicalQuads, "assign", "x", "", "0");
    Emit(logicalQuads, "label", "", "", L_done);

    Surab::Log("{:=^10}", "");
    Surab::Log("short-circuit evaluation: (a && b)");
    PrintQuadruples(logicalQuads);
    Surab::Log("{:=^10}", "");

    Footer();
    return 0;
}
