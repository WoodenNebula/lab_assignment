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
    std::set<std::string> def;
    std::set<std::string> use;
    bool isJump = false;
    bool isConditionalJump = false;
    bool isReturn = false;
    bool isWrite = false;
    bool isLabel = false;

    std::string ToString() const {
        std::ostringstream oss;
        oss << "(" << op << ", " << arg1 << ", " << arg2 << ", " << result << ")";
        return oss.str();
    }
};

std::vector<std::string> ExtractSymbols(const std::string& expr) {
    std::vector<std::string> ids;
    std::string current;
    for (char ch : expr) {
        if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_' || ch == '.') {
            current += ch;
        }
        else {
            if (!current.empty()) {
                ids.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) ids.push_back(current);
    return ids;
}

bool IsNumericLiteral(const std::string& value) {
    if (value.empty()) return false;
    if (value[0] == '-' || value[0] == '+') {
        return value.size() > 1 && value.find_first_not_of("-+0123456789") == std::string::npos;
    }
    return value.find_first_not_of("0123456789") == std::string::npos;
}

std::string StripLabel(const std::string& line) {
    auto pos = line.find(':');
    if (pos != std::string::npos) return Surab::Trim(line.substr(pos + 1));
    return Surab::Trim(line);
}

std::vector<Quadruple> ParseTAC(const std::vector<std::string>& lines) {
    std::vector<Quadruple> program;
    for (const auto& rawLine : lines) {
        std::string raw = Surab::Trim(rawLine);
        if (raw.empty()) continue;

        std::string labelName;
        if (raw.find(':') != std::string::npos && raw.find("goto") == std::string::npos && raw.find("if") == std::string::npos) {
            labelName = Surab::Trim(raw.substr(0, raw.find(':')));
            raw = StripLabel(raw);
            if (!raw.empty()) {
                std::vector<std::string> tokens = { "label", "", "", labelName };
                // label is emitted before the instruction it prefixes
            }
            if (raw.empty()) {
                Quadruple labelQuad{ "label", "", "", labelName };
                labelQuad.isLabel = true;
                program.push_back(labelQuad);
                continue;
            }
        }

        Quadruple quad;
        if (labelName != "") {
            Quadruple labelQuad{ "label", "", "", labelName };
            labelQuad.isLabel = true;
            program.push_back(labelQuad);
        }

        if (raw.find("goto") != std::string::npos) {
            quad.op = "goto";
            quad.isJump = true;
            quad.result = Surab::Trim(raw.substr(raw.find("goto") + 4));
            if (!quad.result.empty()) {
                quad.use.insert(quad.result);
            }
        }
        else if (raw.find("if") == 0) {
            quad.op = "if_false";
            quad.isJump = true;
            quad.isConditionalJump = true;
            auto gotoPos = raw.find("goto");
            if (gotoPos != std::string::npos) {
                std::string condition = Surab::Trim(raw.substr(2, gotoPos - 2));
                quad.arg1 = condition;
                for (const auto& sym : ExtractSymbols(condition)) {
                    if (!IsNumericLiteral(sym) && sym != "if" && sym != "goto") quad.use.insert(sym);
                }
                quad.result = Surab::Trim(raw.substr(gotoPos + 4));
                if (!quad.result.empty()) quad.use.insert(quad.result);
            }
        }
        else if (raw.find("return") == 0) {
            quad.op = "return";
            quad.isReturn = true;
            quad.arg1 = Surab::Trim(raw.substr(6));
            for (const auto& sym : ExtractSymbols(quad.arg1)) {
                if (!IsNumericLiteral(sym)) quad.use.insert(sym);
            }
        }
        else if (raw.find("print") == 0 || raw.find("write") == 0) {
            quad.op = "write";
            quad.isWrite = true;
            quad.arg1 = Surab::Trim(raw.substr(raw.find_first_of(" \t") + 1));
            for (const auto& sym : ExtractSymbols(quad.arg1)) {
                if (!IsNumericLiteral(sym)) quad.use.insert(sym);
            }
        }
        else if (raw.find('=') != std::string::npos) {
            auto eq = raw.find('=');
            std::string lhs = Surab::Trim(raw.substr(0, eq));
            std::string rhs = Surab::Trim(raw.substr(eq + 1));
            quad.op = "=";
            quad.arg1 = rhs;
            quad.result = lhs;
            quad.def.insert(lhs);
            for (const auto& sym : ExtractSymbols(rhs)) {
                if (!IsNumericLiteral(sym) && sym != lhs) quad.use.insert(sym);
            }
        }
        else {
            quad.op = raw;
        }

        if (quad.op.empty()) continue;
        program.push_back(quad);
    }

    return program;
}

std::string FormatQuadruples(const std::vector<Quadruple>& quads) {
    std::ostringstream oss;
    oss << "Quadruples:\n";
    for (size_t i = 0; i < quads.size(); ++i) {
        const auto& q = quads[i];
        oss << "  [" << i << "] " << q.ToString() << "\n";
    }
    return oss.str();
}

std::unordered_map<int, std::vector<int>> BuildCFG(const std::vector<Quadruple>& program) {
    std::unordered_map<int, std::vector<int>> cfg;
    std::unordered_map<std::string, int> labelPos;

    for (size_t i = 0; i < program.size(); ++i) {
        if (program[i].isLabel && !program[i].result.empty()) {
            labelPos[program[i].result] = static_cast<int>(i) + 1;
        }
    }

    for (size_t i = 0; i < program.size(); ++i) {
        cfg[i + 1] = {};
        const auto& q = program[i];

        if (q.isReturn || q.op == "label") {
            if (i + 1 < program.size()) cfg[i + 1].push_back(i + 2);
            continue;
        }

        if (q.isConditionalJump) {
            if (!q.result.empty() && labelPos.count(q.result)) cfg[i + 1].push_back(labelPos[q.result]);
            if (i + 1 < program.size()) cfg[i + 1].push_back(i + 2);
        }
        else if (q.isJump) {
            if (!q.result.empty() && labelPos.count(q.result)) cfg[i + 1].push_back(labelPos[q.result]);
        }
        else if (i + 1 < program.size()) {
            cfg[i + 1].push_back(i + 2);
        }
    }

    for (auto& [node, edges] : cfg) {
        std::sort(edges.begin(), edges.end());
        edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
    }

    return cfg;
}

void PrintCFG(const std::unordered_map<int, std::vector<int>>& cfg) {
    std::vector<int> keys = cfg | std::views::keys | std::ranges::to<std::vector>();
    std::ranges::sort(keys);

    Surab::Log("Basic block leaders: {}", Surab::ToString(keys, ", ", true));
    Surab::Log("CFG edges:");
    for (int key : keys) {
        std::string value;
        for (size_t i = 0; i < cfg.at(key).size(); ++i) {
            value += std::to_string(cfg.at(key)[i]) + (i + 1 < cfg.at(key).size() ? ", " : "");
        }
        Surab::Log("Block {} -> {}", key, value.empty() ? "[]" : value);
    }
}

std::unordered_map<int, std::set<std::string>> ComputeLiveSets(const std::vector<Quadruple>& program,
    const std::unordered_map<int, std::vector<int>>& cfg) {
    std::unordered_map<int, std::set<std::string>> in, out;
    bool changed = true;

    while (changed) {
        changed = false;
        for (int i = static_cast<int>(program.size()) - 1; i >= 0; --i) {
            std::set<std::string> outSet;
            for (int succ : cfg.at(i + 1)) {
                outSet.insert(in[succ].begin(), in[succ].end());
            }

            std::set<std::string> inSet = program[i].use;
            for (const auto& var : outSet) {
                if (!program[i].def.count(var)) inSet.insert(var);
            }

            if (inSet != in[i + 1] || outSet != out[i + 1]) {
                in[i + 1] = inSet;
                out[i + 1] = outSet;
                changed = true;
            }
        }
    }

    return in;
}

std::set<int> FindReachable(const std::unordered_map<int, std::vector<int>>& cfg) {
    std::set<int> reachable = { 1 };
    std::vector<int> stack = { 1 };

    while (!stack.empty()) {
        int node = stack.back();
        stack.pop_back();
        for (int succ : cfg.at(node)) {
            if (reachable.insert(succ).second) stack.push_back(succ);
        }
    }
    return reachable;
}

void RemoveDeadCode(std::vector<Quadruple>& program) {
    std::unordered_map<int, std::vector<int>> cfg = BuildCFG(program);
    std::set<int> reachable = FindReachable(cfg);

    std::vector<Quadruple> kept;
    std::vector<std::string> removed;

    for (size_t i = 0; i < program.size(); ++i) {
        if (reachable.count(static_cast<int>(i) + 1) == 0) {
            removed.push_back(program[i].ToString());
            continue;
        }
        kept.push_back(program[i]);
    }

    std::vector<Quadruple> reachableProgram = kept;
    std::unordered_map<int, std::vector<int>> reachableCFG = BuildCFG(reachableProgram);
    auto live = ComputeLiveSets(reachableProgram, reachableCFG);

    std::vector<Quadruple> finalProgram;
    for (size_t i = 0; i < reachableProgram.size(); ++i) {
        const auto& quad = reachableProgram[i];
        const auto it = live.find(static_cast<int>(i) + 1);

        bool keepThis = true;
        if (quad.isReturn || quad.isWrite) {
            keepThis = true;
        }
        else if (quad.op == "label") {
            keepThis = true;
        }
        else if (it != live.end() && it->second.empty() && quad.def.empty()) {
            keepThis = false;
        }

        if (keepThis) {
            finalProgram.push_back(quad);
        }
        else {
            removed.push_back(quad.ToString());
        }
    }

    program = finalProgram;

    Surab::Log("Removed instructions:");
    if (removed.empty()) {
        Surab::Log("  none");
    }
    else {
        for (const auto& item : removed) Surab::Log("  - {}", item);
    }
}

int main() {
    Header("Dead Code Elimination");

    std::vector<std::string> tac = {
        "i = 1",
        "L1: t1 = i * 4",
        "if t1 > 100 goto L2",
        "dead = 999",
        "goto L1",
        "L2: return i"
    };

    std::vector<Quadruple> program = ParseTAC(tac);

    Surab::Log("{}", FormatQuadruples(program));

    auto cfg = BuildCFG(program);
    PrintCFG(cfg);

    RemoveDeadCode(program);

    Surab::Log("Final TAC after DCE:");
    for (size_t i = 0; i < program.size(); ++i) Surab::Log("{}: {}", i + 1, program[i].ToString());

    Footer();
    return 0;
}
