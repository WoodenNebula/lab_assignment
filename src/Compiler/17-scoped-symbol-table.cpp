#include "src/commons.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct SymbolInfo {
    std::string name;
    std::string type;
    int size = 0;
    int scopeLevel = 0;
};

class ScopedSymbolTable {
public:
    ScopedSymbolTable() {}

    void enter_scope() { m_ScopeStack.push_back({}); }

    void leave_scope() {
        if (m_ScopeStack.size() > 1) { m_ScopeStack.pop_back(); }
        else { Surab::LogError("Cannot leave the global scope."); }
    }

    bool insert(const std::string& name, const SymbolInfo& info) {
        auto& currentScope = m_ScopeStack.back();
        if (currentScope.find(name) != currentScope.end()) {
            Surab::LogError("Redeclaration Error: {} in current scope", name);
            return false;
        }

        currentScope[name] = info;
        return true;
    }

    SymbolInfo* lookup(const std::string& name) {
        for (int i = static_cast<int>(m_ScopeStack.size()) - 1; i >= 0; --i) {
            auto it = m_ScopeStack[i].find(name);
            if (it != m_ScopeStack[i].end()) {
                return &it->second;
            }
        }

        Surab::LogError("Undeclared Error: {}", name);
        return nullptr;
    }

    void PrintScopes() const {
        for (size_t level = 0; level < m_ScopeStack.size(); ++level) {
            Surab::Log("Level {}:", level);
            for (const auto& [name, info] : m_ScopeStack[level]) {
                Surab::Log("{}: type={}, size={}, scope={}", name, info.type, info.size, info.scopeLevel);
            }
        }
    }

    size_t GetScopeCount() const {
        return m_ScopeStack.size();
    }
private:
    std::vector<std::unordered_map<std::string, SymbolInfo>> m_ScopeStack;
};

struct BlockNode {
    // "block_entry", "block_exit", "declare", "lookup"
    std::string Kind;

    std::string Name;
    std::string Type;
    int Size = 0;
};

void SimulateAST(ScopedSymbolTable& table, const std::vector<BlockNode>& ast) {
    for (const auto& node : ast) {
        if (node.Kind == "block_entry") {
            table.enter_scope();
            Surab::Log("AST: Enter block '{}'", node.Name);
        }
        else if (node.Kind == "declare") {
            SymbolInfo info{ node.Name, node.Type, node.Size, static_cast<int>(table.GetScopeCount() - 1) };
            table.insert(node.Name, info);
            Surab::Log("AST: Declare '{}' of type '{}'", node.Name, node.Type);

            Surab::Log("{}{:=^28}", GREY, " Symbol Table ");
            table.PrintScopes();
            Surab::Log("{:=^28}{}", "", RESET);
        }
        else if (node.Kind == "lookup") {
            auto* found = table.lookup(node.Name);
            if (found) {
                Surab::LogSuccess("AST: Lookup '{}' -> type={}, size={}, scope={}", found->name, found->type, found->size, found->scopeLevel);
            }
        }
        else if (node.Kind == "block_exit") {
            table.leave_scope();
            Surab::Log("AST: Exit block '{}'", node.Name);
        }
    }
}

int main() {
    Header("Scoped Symbol Table");

    ScopedSymbolTable table;

    // table.insert("globalVar", { "globalVar", "int", 4, 0 });

    std::vector<BlockNode> ast = {
        { "block_entry", "global", "", 0 },
        { "block_entry", "outer", "", 0 },
        { "declare", "x", "int", 4 },
        { "declare", "y", "float", 8 },
        { "lookup", "x", "", 0 },
        { "block_entry", "inner", "", 0 },
        { "declare", "x", "float", 8 },
        { "lookup", "x", "", 0 },
        { "block_exit", "inner", "", 0 },
        { "lookup", "y", "", 0 },
        { "block_exit", "outer", "", 0 },
    };
    SimulateAST(table, ast);

    Footer();
    return 0;
}
