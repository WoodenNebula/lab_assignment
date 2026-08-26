/*
Construct SLR(1) parsing table for the classic expression grammar:
S' -> E
E -> E + T | T
T -> T * F | F
F -> ( E ) | id

This program computes LR(0) canonical items and prints the complete SLR(1)
ACTION and GOTO tables.
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <map>
#include <print>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Token_t = Surab::Compiler::Token_t;
using Grammar_t = Surab::Compiler::Grammar_t;
using GrammarList_t = Surab::Compiler::GrammarList_t;
using SSet_t = Surab::Compiler::SSet_t;
using Item_t = std::pair<size_t, size_t>;
using ItemSet_t = std::set<Item_t>;
using ActionTable_t = std::map<size_t, std::map<Token_t, std::string>>;
using GotoTable_t = std::map< size_t, std::map<Token_t, size_t>>;

struct SCanonicalState {
    size_t StateId;
    size_t CalleeStateId;
    Token_t CalleeSymbol;
    ItemSet_t Items;
};

bool IsNonTerminal(const Token_t& symbol, const SSet_t& nonTerminals) {
    return nonTerminals.contains(symbol);
}

std::string FormatItem(const GrammarList_t& grammar, const Item_t& item) {
    const auto& [prodIndex, dot] = item;
    const auto& [lhs, rhs] = grammar.at(prodIndex);
    std::string result = lhs + " -> ";
    for (size_t i = 0; i < rhs.size(); ++i) {
        if (i == dot)
            result += DOT;
        result += rhs[i];
    }
    if (dot == rhs.size())
        result += DOT;
    return result;
}

void PrintCanonicalStates(const GrammarList_t& GrammarList, const std::vector<SCanonicalState>& States) {
    Surab::Log("\nCanonical LR(0) collection states:");

    for (size_t i = 0; i < States.size(); ++i) {
        std::string printString = std::format("I[{}]: ", i);

        const auto& [StateId, CalleeStateId, CalleeSymbol, Items] = States[i];
        printString += std::format("{}({}, {}) = ", StateId == 0 ? "Closure" : "Goto", CalleeStateId, CalleeSymbol);

        printString += "[ ";
        for (const auto& item : Items) {
            printString += FormatItem(GrammarList, item) + ", ";
        }

        // Remove the last ", "
        printString.erase(printString.size() - 2, 2);
        printString += " ]";
        Surab::Log("{}", printString);
    }
}

std::string StateKey(const GrammarList_t& GrammarList, const ItemSet_t& State) {
    std::string key;
    for (const auto& item : State) {
        if (!key.empty())
            key += " | ";
        key += FormatItem(GrammarList, item);
    }
    return key;
}

ItemSet_t ClosureLR0(const ItemSet_t& State, const GrammarList_t& GrammarList, const SSet_t& NonTerminals) {
    ItemSet_t closure = State;
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto& [prodIndex, dot] : closure) {
            if (prodIndex >= GrammarList.size())
                continue;

            const auto& [lhs, rhs] = GrammarList[prodIndex];
            if (dot >= rhs.size())
                continue;

            const Token_t symbol = rhs[dot];
            if (!IsNonTerminal(symbol, NonTerminals))
                continue;

            // Add all productions of the non-terminal symbol to the closure
            for (size_t i = 0; i < GrammarList.size(); ++i) {
                const auto& [candidateLhs, candidateRhs] = GrammarList[i];
                if (candidateLhs != symbol)
                    continue;
                const Item_t next = { i, 0 };
                if (closure.insert(next).second)
                    changed = true;
            }
        }
    }

    return closure;
}

ItemSet_t GotoLR0(const ItemSet_t& State, const Token_t& Symbol, const GrammarList_t& GrammarList, const SSet_t& NonTerminals) {
    ItemSet_t movedState;
    for (const auto& [prodIndex, dot] : State) {
        const auto& [lhs, rhs] = GrammarList[prodIndex];
        bool isValidIndex = dot < rhs.size();
        if (!isValidIndex)
            continue;
        bool isMatchingSymbol = rhs[dot] == Symbol;
        if (isMatchingSymbol) {
            movedState.insert({ prodIndex, dot + 1 });
        }
    }
    return ClosureLR0(movedState, GrammarList, NonTerminals);
}

std::vector<SCanonicalState> CanonicalCollectionLR0(const GrammarList_t& GrammarList, const SSet_t& NonTerminals) {
    std::vector<SCanonicalState> states;
    std::vector<ItemSet_t> queue;

    ItemSet_t start = ClosureLR0({ { 0, 0 } }, GrammarList, NonTerminals);
    queue.push_back(start);
    states.push_back({ .StateId = 0, .CalleeStateId = 0, .CalleeSymbol = GrammarList[0].first, .Items = start });

    std::map<std::string, size_t> seen;
    seen[StateKey(GrammarList, start)] = 0;

    for (size_t index = 0; index < queue.size(); ++index) {
        const ItemSet_t current = queue[index];
        std::vector<Token_t> symbols;
        // collect all the unique symbols that are immediately after the dot in the current state
        for (const auto& [prodIndex, dot] : current) {
            const auto& [lhs, rhs] = GrammarList[prodIndex];
            if (dot < rhs.size() && !std::ranges::contains(symbols, rhs[dot])) {
                symbols.push_back(rhs[dot]);
            }
        }

        for (const auto& symbol : symbols) {
            ItemSet_t next = GotoLR0(current, symbol, GrammarList, NonTerminals);
            if (next.empty())
                continue;
            const std::string key = StateKey(GrammarList, next);
            if (!seen.contains(key)) {
                seen[key] = states.size();
                states.push_back({ .StateId = states.size(), .CalleeStateId = index, .CalleeSymbol = symbol, .Items = next });
                queue.push_back(next);
            }
        }
    }

    return states;
}




ActionTable_t ConstructActionTable(
    const std::vector<SCanonicalState>& States,
    const GrammarList_t& GrammarList,
    const Token_t& StartSymbol,
    const std::map<Token_t, SSet_t>& FollowMap,
    const SSet_t& NonTerminals
) {
    ActionTable_t actionTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const ItemSet_t state = States[stateId].Items;
        for (const auto& item : state) {
            const auto& [prodIndex, dot] = item;
            const auto& [lhs, rhs] = GrammarList[prodIndex];

            if (dot == rhs.size()) {
                if (lhs == StartSymbol + "'") {
                    actionTable[stateId]["$"] = "accept";
                }
                else {
                    for (const auto& lookahead : FollowMap.at(lhs)) {
                        actionTable[stateId][lookahead] = "reduce: " + Surab::Compiler::FormatProduction({ lhs, rhs });
                    }
                }
                continue;
            }

            const Token_t symbol = rhs[dot];
            if (!IsNonTerminal(symbol, NonTerminals)) {
                ItemSet_t shifted = GotoLR0(state, symbol, GrammarList, NonTerminals);
                size_t target = std::numeric_limits<size_t>::max();
                for (size_t i = 0; i < States.size(); ++i) {
                    if (States[i].Items == shifted) {
                        target = i;
                        break;
                    }
                }
                if (target != std::numeric_limits<size_t>::max()) {
                    actionTable[stateId][symbol] = "shift: " + StartSymbol + std::to_string(target);
                }
            }
        }
    }
    return actionTable;
}

GotoTable_t ConstructGotoTable(
    const std::vector<SCanonicalState>& States,
    const GrammarList_t& GrammarList,
    const SSet_t& NonTerminals
) {
    GotoTable_t gotoTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const ItemSet_t state = States[stateId].Items;

        for (const auto& nt : NonTerminals) {
            ItemSet_t shifted = GotoLR0(state, nt, GrammarList, NonTerminals);
            if (shifted.empty()) continue;
            for (size_t i = 0; i < States.size(); ++i) {
                if (States[i].Items == shifted) {
                    gotoTable[stateId][nt] = i;
                    break;
                }
            }
        }
    }
    return gotoTable;
}

void PrintCombinedParseTable(
    const ActionTable_t& ActionTable,
    const GotoTable_t& GotoTable,
    const SSet_t& NonTerminals,
    const SSet_t& Terminals
) {
    Surab::Log("\nSLR(1) Parsing Table:");

    std::vector<std::string> RowKeys;
    std::vector<std::string> ColumnKeys;

    std::string RowHeader = "States";

    size_t actionTableColumnCount = 0;

    // Collect all row and column keys from both tables
    {
        for (const auto& [stateId, row] : ActionTable) {
            RowKeys.push_back(std::to_string(stateId));
            for (const auto& [terminal, _] : row) {
                if (!std::ranges::contains(ColumnKeys, terminal)) {
                    ColumnKeys.push_back(terminal);
                    ++actionTableColumnCount;
                }
            }
        }

        for (const auto& [stateId, row] : GotoTable) {
            if (!std::ranges::contains(RowKeys, std::to_string(stateId))) {
                RowKeys.push_back(std::to_string(stateId));
            }
            for (const auto& [nonTerminal, _] : row) {
                if (!std::ranges::contains(ColumnKeys, nonTerminal)) {
                    ColumnKeys.push_back(nonTerminal);
                }
            }
        }
    }

    RowKeys.insert(RowKeys.begin(), RowHeader);
    ColumnKeys.insert(ColumnKeys.begin(), RowHeader);

    std::map<std::string, std::map<std::string, std::string>> cells;

    // Build cell values
    for (const auto& rowKey : RowKeys) {
        for (const auto& columnKey : ColumnKeys) {
            std::string cell;
            if (rowKey == RowHeader || columnKey == RowHeader) {
                cell = columnKey == RowHeader ? rowKey : columnKey;
            }
            else {
                if (IsNonTerminal(columnKey, NonTerminals)) {
                    size_t stateId = std::stoi(rowKey);
                    if (GotoTable.contains(stateId) && GotoTable.at(stateId).contains(columnKey)) {
                        cell = std::to_string(GotoTable.at(stateId).at(columnKey));
                    }
                    else {
                        cell = " ";
                    }
                }
                else {
                    size_t stateId = std::stoi(rowKey);
                    if (ActionTable.contains(stateId) && ActionTable.at(stateId).contains(columnKey)) {
                        cell = ActionTable.at(stateId).at(columnKey);
                    }
                    else {
                        cell = "-";
                    }
                }
            }
            cells[rowKey][columnKey] = cell;
        }
    }

    auto funcGetLengthAsString = [](const auto& value) {
        return std::format("{}", value).size();
        };


    // Calculate individual column widths
    std::map<std::string, int> columnWidths;
    {
        for (const std::string& columnKey : ColumnKeys) {
            auto columnCellValues = cells
                | std::views::values
                | std::views::filter([columnKey](const std::map<std::string, std::string>& map) { return map.contains(columnKey); })
                | std::views::join;

            int width = std::ranges::max(columnCellValues | std::views::transform(funcGetLengthAsString));
            columnWidths[columnKey] = width + 2;
        }

        // Calculate row-header width
        int rowWidth = std::ranges::max(RowKeys | std::views::transform(funcGetLengthAsString));
        rowWidth = std::max(rowWidth, (int)RowHeader.size());
        rowWidth += 2;

        columnWidths[RowHeader] = rowWidth;
    }

    // Separator
    auto funcPrintSeparator = [&]() {
        for (const auto& columnKey : ColumnKeys) {
            std::print("+{:-<{}}", "", columnWidths.at(columnKey));
        }
        std::println("+");
        };

    auto funcPrintRow = [&](const std::vector<std::string>& rowValues) {
        for (size_t i = 0; i < rowValues.size(); ++i) {
            const auto& columnKey = ColumnKeys[i];
            std::print("|{:^{}}", rowValues[i], columnWidths.at(columnKey));
        }
        std::println("|");
        };

    funcPrintSeparator();
    // Pre-Header
    {
        std::string actionTableHeader = "Action Table";
        std::string gotoTableHeader = "Goto Table";

        std::print("|{:^{}}", "", columnWidths.at(RowHeader));
        size_t actionTableColumnWidth = 0;
        size_t gotoTableColumnWidth = 0;
        for (size_t i = 1; i < ColumnKeys.size(); ++i) {
            if (i <= actionTableColumnCount)
                actionTableColumnWidth += columnWidths.at(ColumnKeys[i]);
            else
                gotoTableColumnWidth += columnWidths.at(ColumnKeys[i]);
        }

        actionTableColumnWidth = std::max(actionTableHeader.size(), actionTableColumnWidth);
        // Add padding of the '|'
        // -1 for the first column which is the row header
        actionTableColumnWidth += actionTableColumnCount - 1;
        std::print("|{:^{}}", actionTableHeader, actionTableColumnWidth);

        gotoTableColumnWidth = std::max(gotoTableHeader.size(), gotoTableColumnWidth);
        // Add padding of the '|' after the action table
        // ColumnKeys.size() - actionTableColumnCount gives the number of columns in the goto table + row header
        // -2 for the first column which is the row header and the last column which is the separator
        gotoTableColumnWidth += ColumnKeys.size() - actionTableColumnCount - 2;
        std::print("|{:^{}}", gotoTableHeader, gotoTableColumnWidth);

        std::println("|");
    }

    funcPrintSeparator();
    for (const auto& rowKey : RowKeys) {
        auto rowValues = ColumnKeys
            | std::views::transform([&](const auto& columnKey) { return cells.contains(rowKey) && cells.at(rowKey).contains(columnKey) ? cells.at(rowKey).at(columnKey) : "-"; })
            | std::ranges::to<std::vector<std::string>>();

        funcPrintRow(rowValues);
        funcPrintSeparator();
    }

}

int main() {
    Header("SLR(1) Parsing Table");

    std::stringstream grammarString(R"(
        E -> E+T | T
        T -> T*F | F
        F -> (E) | id
    )");

    const Token_t startSymbol = "E";
    const Token_t augmentedStartSymbol = startSymbol + "'";

    Grammar_t augmentedGrammar = Surab::Compiler::AugmentGrammar(Surab::Compiler::ParseGrammarFromString(grammarString), startSymbol);
    Surab::Log("\nAugmented Grammar:");
    Surab::Compiler::PrintGrammar(augmentedGrammar);

    const auto firstMap = Surab::Compiler::ComputeFirst(augmentedGrammar);
    const auto followMap = Surab::Compiler::ComputeFollow(augmentedGrammar, firstMap, augmentedStartSymbol);

    const auto [nonTerminals, rawTerminals] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(augmentedGrammar);
    const auto grammarList = Surab::Compiler::ToProductionVector(augmentedGrammar, startSymbol);

    Surab::Log("\nGrammar List: ");
    Surab::Compiler::PrintGrammarList(grammarList);

    const auto states = CanonicalCollectionLR0(grammarList, nonTerminals);
    PrintCanonicalStates(grammarList, states);

    Surab::Log("\nFOLLOW sets:");
    for (auto& [A, _] : followMap) {
        Surab::LogSuccess("FOLLOW({}) = {} ", A, followMap.at(A));
    }

    ActionTable_t actionTable = ConstructActionTable(states, grammarList, startSymbol, followMap, nonTerminals);
    GotoTable_t gotoTable = ConstructGotoTable(states, grammarList, nonTerminals);

    PrintCombinedParseTable(actionTable, gotoTable, nonTerminals, rawTerminals);


    Footer();
    return 0;
}
