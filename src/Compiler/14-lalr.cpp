#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <print>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using GrammarClass = Surab::Compiler::Grammar::GrammarClass;

using Token_t = Surab::Compiler::Token_t;
using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;

using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;

using ActionTable_t = Surab::Compiler::Parser::ActionTable_t;
using GotoTable_t = Surab::Compiler::Parser::GotoTable_t;

using LR1Item = Surab::Compiler::LR1::ParserItem;
using LR1ItemSet = std::set<LR1Item>;
using LR1CanonicalState = Surab::Compiler::LR1::ParserCanonicalState;

void PrintCombinedParseTable(
    const GrammarClass& Grammar,
    const ActionTable_t& ActionTable,
    const GotoTable_t& GotoTable,
    const std::vector<std::vector<size_t>>& MergedGroups = {}
) {
    std::vector<std::string> RowKeys;
    std::vector<std::string> ColumnKeys;

    std::string RowHeader = "States";

    // Collect and sort state IDs
    {
        std::set<size_t> stateIdSet;
        for (const auto& [stateId, _] : ActionTable) {
            stateIdSet.insert(stateId);
        }
        for (const auto& [stateId, _] : GotoTable) {
            stateIdSet.insert(stateId);
        }

        for (size_t id : stateIdSet) {
            RowKeys.push_back(std::to_string(id));
        }
    }

    std::vector<std::string> actionColumns, gotoColumns;
    // Collect action and goto columns labels
    {
        // Action columns
        const auto& grammarTerminals = Grammar.GetTerminals();
        for (const auto& t : grammarTerminals) {
            if (t != EPSILON && !std::ranges::contains(actionColumns, t)) {
                actionColumns.push_back(t);
            }
        }
        if (!std::ranges::contains(actionColumns, "$")) {
            actionColumns.push_back("$");
        }
        for (const auto& [_, row] : ActionTable) {
            for (const auto& [term, _] : row) {
                if (term != EPSILON && !std::ranges::contains(actionColumns, term)) {
                    actionColumns.push_back(term);
                }
            }
        }

        // Goto columns (NonTerminals excluding augmented start symbol)
        const auto& grammarNonTerminals = Grammar.GetNonTerminals();
        const auto startSymbol = Grammar.GetStartSymbol();
        for (const auto& nt : grammarNonTerminals) {
            if (nt != startSymbol && !std::ranges::contains(gotoColumns, nt)) {
                gotoColumns.push_back(nt);
            }
        }
        for (const auto& [_, row] : GotoTable) {
            for (const auto& [nt, _] : row) {
                if (!std::ranges::contains(gotoColumns, nt)) {
                    gotoColumns.push_back(nt);
                }
            }
        }

        // Build complete ColumnKeys list
        ColumnKeys.push_back(RowHeader);
        for (const auto& col : actionColumns) { ColumnKeys.push_back(col); }
        for (const auto& col : gotoColumns) { ColumnKeys.push_back(col); }
    }


    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> cells;

    // Populate cells data
    {
        // get new and merged States label
        auto funcGetFormattedStateLabel = [&](size_t stateId) -> std::string {
            if (stateId < MergedGroups.size() && !MergedGroups[stateId].empty()) {
                std::string membersStr;
                for (size_t i = 0; i < MergedGroups[stateId].size(); ++i) {
                    if (i > 0) membersStr += ", ";
                    membersStr += std::format("I{}", MergedGroups[stateId][i]);
                }
                return std::format("{} ({})", stateId, membersStr);
            }
            return std::to_string(stateId);
            };

        // get new and merged states
        auto funcGetFormattedGotoLabel = [&](size_t targetStateId) -> std::string {
            if (targetStateId < MergedGroups.size() && !MergedGroups[targetStateId].empty()) {
                std::string membersStr;
                for (size_t i = 0; i < MergedGroups[targetStateId].size(); ++i) {
                    if (i > 0) membersStr += ", ";
                    membersStr += std::format("I{}", MergedGroups[targetStateId][i]);
                }
                return std::format("{} [{}]", targetStateId, membersStr);
            }
            return std::to_string(targetStateId);
            };

        for (const auto& rowKey : RowKeys) {
            size_t stateId = std::stoi(rowKey);
            std::string stateLabel = funcGetFormattedStateLabel(stateId);

            for (const auto& colKey : ColumnKeys) {
                if (colKey == RowHeader) {
                    cells[rowKey][colKey] = stateLabel;
                }
                else if (std::ranges::contains(gotoColumns, colKey)) {
                    if (GotoTable.contains(stateId) && GotoTable.at(stateId).contains(colKey)) {
                        size_t targetStateId = GotoTable.at(stateId).at(colKey);
                        cells[rowKey][colKey] = funcGetFormattedGotoLabel(targetStateId);
                    }
                    else {
                        cells[rowKey][colKey] = " ";
                    }
                }
                else {
                    // Action column
                    if (ActionTable.contains(stateId) && ActionTable.at(stateId).contains(colKey)) {
                        cells[rowKey][colKey] = ActionTable.at(stateId).at(colKey);
                    }
                    else {
                        cells[rowKey][colKey] = "-";
                    }
                }
            }
        }
    }

    std::string actionTableHeader = "Action Table";
    std::string gotoTableHeader = "Goto Table";

    size_t actionTableColumnCount = actionColumns.size();
    size_t gotoColumnCount = ColumnKeys.size() - 1 - actionTableColumnCount;

    int actionSeparators, gotoSeparators;

    std::unordered_map<std::string, int> columnWidths;
    // Calculate column widths for ASCII printing
    {
        std::vector<std::string> allRowKeys;
        allRowKeys.push_back(RowHeader);
        for (const auto& r : RowKeys) {
            allRowKeys.push_back(r);
        }

        int minColWidth = 9;
        auto funcGetLengthAsString = [](const std::string& value) -> int {
            return static_cast<int>(value.size());
            };

        for (const std::string& colKey : ColumnKeys) {
            int maxLen = 0;
            for (const auto& rKey : allRowKeys) {
                if (cells.contains(rKey) && cells.at(rKey).contains(colKey)) {
                    maxLen = std::max(maxLen, funcGetLengthAsString(cells.at(rKey).at(colKey)));
                }
                else if (rKey == RowHeader) {
                    maxLen = std::max(maxLen, funcGetLengthAsString(colKey));
                }
                maxLen = std::max(maxLen, minColWidth);
            }
            columnWidths[colKey] = maxLen + 2;
        }

        // Guarantee section header widths fit without breaking grid layout
        int currentActionWidth = 0;
        for (size_t i = 1; i <= actionTableColumnCount; ++i) {
            currentActionWidth += columnWidths[ColumnKeys[i]];
        }
        actionSeparators = (actionTableColumnCount > 0) ? static_cast<int>(actionTableColumnCount - 1) : 0;
        if (currentActionWidth + actionSeparators < static_cast<int>(actionTableHeader.size())) {
            int diff = static_cast<int>(actionTableHeader.size()) - (currentActionWidth + actionSeparators);
            if (actionTableColumnCount > 0) {
                columnWidths[ColumnKeys[actionTableColumnCount]] += diff;
            }
        }

        int currentGotoWidth = 0;
        for (size_t i = actionTableColumnCount + 1; i < ColumnKeys.size(); ++i) {
            currentGotoWidth += columnWidths[ColumnKeys[i]];
        }
        gotoSeparators = (gotoColumnCount > 0) ? static_cast<int>(gotoColumnCount - 1) : 0;
        if (currentGotoWidth + gotoSeparators < static_cast<int>(gotoTableHeader.size())) {
            int diff = static_cast<int>(gotoTableHeader.size()) - (currentGotoWidth + gotoSeparators);
            if (gotoColumnCount > 0) {
                columnWidths[ColumnKeys.back()] += diff;
            }
        }
    }


    // Grid printing lambdas
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

    //////////////////
    // Print table //
    /////////////////
    funcPrintSeparator();

    // Pre-Header Row
    {
        std::print("|{:^{}}", "", columnWidths.at(RowHeader));

        int actionSpanWidth = 0;
        for (size_t i = 1; i <= actionTableColumnCount; ++i) {
            actionSpanWidth += columnWidths.at(ColumnKeys[i]);
        }
        actionSpanWidth += actionSeparators;
        std::print("|{:^{}}", actionTableHeader, actionSpanWidth);

        int gotoSpanWidth = 0;
        for (size_t i = actionTableColumnCount + 1; i < ColumnKeys.size(); ++i) {
            gotoSpanWidth += columnWidths.at(ColumnKeys[i]);
        }
        gotoSpanWidth += gotoSeparators;
        if (gotoColumnCount > 0) {
            std::print("|{:^{}}", gotoTableHeader, gotoSpanWidth);
        }

        std::println("|");
    }

    funcPrintSeparator();

    // Column Headers Row
    {
        std::vector<std::string> headerValues;
        for (const auto& colKey : ColumnKeys) {
            headerValues.push_back(colKey);
        }
        funcPrintRow(headerValues);
        funcPrintSeparator();
    }

    // Table Data Rows
    for (const auto& rowKey : RowKeys) {
        std::vector<std::string> rowValues;
        for (const auto& colKey : ColumnKeys) {
            rowValues.push_back(cells.at(rowKey).at(colKey));
        }
        funcPrintRow(rowValues);
        funcPrintSeparator();
    }
}

OrderedTokenSet FirstOfSequence(
    const std::vector<Token_t>& Sequence,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap,
    const OrderedTokenSet& NonTerminals
) {
    OrderedTokenSet result;
    bool nullable = true;

    for (const auto& symbol : Sequence) {
        if (symbol == EPSILON) {
            result.insert(EPSILON);
            continue;
        }

        if (NonTerminals.contains(symbol)) {
            for (const auto& x : FirstMap.at(symbol)) {
                if (x != EPSILON) result.insert(x);
            }
            if (!FirstMap.at(symbol).contains(EPSILON)) {
                nullable = false;
                break;
            }
        }
        else {
            result.insert(symbol);
            nullable = false;
            break;
        }
    }

    if (nullable) {
        result.insert(EPSILON);
    }

    return result;
}

LR1ItemSet ClosureLR1(
    const GrammarClass& AugmentedGrammar,
    const LR1ItemSet& State,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const auto GrammarList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet NonTerminals = AugmentedGrammar.GetNonTerminals();

    LR1ItemSet closure = State;
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto& [prodIndex, dotIndex, lookahead] : closure) {
            if (prodIndex >= GrammarList.size()) {
                continue;
            }

            const TokenizedProduction_t production = GrammarList[prodIndex].TokenizedProductionList[0];
            if (dotIndex >= production.size()) {
                continue;
            }

            const Token_t nextSymbol = production[dotIndex];
            if (!NonTerminals.contains(nextSymbol)) {
                continue;
            }

            std::vector<Token_t> beta(production.begin() + static_cast<std::ptrdiff_t>(dotIndex + 1), production.end());
            beta.push_back(lookahead);
            const OrderedTokenSet firstBeta = FirstOfSequence(beta, FirstMap, NonTerminals);

            for (size_t i = 0; i < GrammarList.size(); ++i) {
                const auto& [candidateSymbol, _] = GrammarList[i];
                if (candidateSymbol != nextSymbol) {
                    continue;
                }

                for (const auto& a : firstBeta) {
                    if (a == EPSILON) {
                        continue;
                    }

                    const LR1Item next = { i, 0, a };
                    if (closure.insert(next).second) {
                        changed = true;
                    }
                }
            }
        }
    }

    return closure;
}

LR1ItemSet GotoLR1(
    const GrammarClass& AugmentedGrammar,
    const LR1ItemSet& State,
    const Token_t& Symbol,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const auto GrammarList = AugmentedGrammar.GetSplitProductions();
    LR1ItemSet movedState;

    for (const auto& [prodIndex, dotIndex, lookAhead] : State) {
        const TokenizedProduction_t& production = GrammarList[prodIndex].TokenizedProductionList[0];
        if (dotIndex < production.size() && production[dotIndex] == Symbol) {
            movedState.insert({ prodIndex, dotIndex + 1, lookAhead });
        }
    }

    return ClosureLR1(AugmentedGrammar, movedState, FirstMap);
}

std::string CoreKey(const LR1ItemSet& State) {
    std::set<std::pair<size_t, size_t>> core;

    for (const auto& [prodIndex, dotIndex, _] : State) {
        core.insert({ prodIndex, dotIndex });
    }

    std::string key;
    for (const auto& [prodIndex, dotIndex] : core) {
        if (!key.empty()) {
            key += " | ";
        }
        key += std::to_string(prodIndex) + ":" + std::to_string(dotIndex);
    }
    return key;
}

std::vector<LR1CanonicalState> CanonicalCollectionLR1(
    const GrammarClass& AugmentedGrammar,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const auto& GrammarList = AugmentedGrammar.GetSplitProductions();
    std::vector<LR1CanonicalState> states;
    std::vector<LR1ItemSet> queue;

    LR1ItemSet start = ClosureLR1(AugmentedGrammar, { { 0, 0, "$" } }, FirstMap);
    queue.push_back(start);
    states.emplace_back(0, 0, "$", start);

    std::unordered_map<std::string, size_t> seen;
    seen[Surab::Compiler::LR1::StateKey(AugmentedGrammar, start)] = 0;

    for (size_t index = 0; index < queue.size(); ++index) {
        const LR1ItemSet current = queue[index];
        OrderedTokenSet symbols;

        for (const auto& [prodIndex, dotIndex, lookahead] : current) {
            const TokenizedProduction_t& production = GrammarList[prodIndex].TokenizedProductionList[0];
            if (dotIndex < production.size()) {
                const Token_t symbol = production[dotIndex];
                if (symbol != EPSILON && !symbols.contains(symbol)) {
                    symbols.push_back(symbol);
                }
            }
        }

        for (const auto& symbol : symbols) {
            LR1ItemSet next = GotoLR1(AugmentedGrammar, current, symbol, FirstMap);
            if (next.empty()) {
                continue;
            }

            const std::string key = Surab::Compiler::LR1::StateKey(AugmentedGrammar, next);
            if (!seen.contains(key)) {
                seen[key] = states.size();
                states.emplace_back(states.size(), index, symbol, next);
                queue.push_back(next);
            }
        }
    }

    return states;
}

std::vector<std::vector<size_t>> ComputeMergedStateGroups(const std::vector<LR1CanonicalState>& States) {
    std::map<std::string, std::vector<size_t>> grouped;
    for (size_t i = 0; i < States.size(); ++i) {
        grouped[CoreKey(States[i].Items)].push_back(i);
    }

    std::vector<std::vector<size_t>> mergedGroups;
    mergedGroups.reserve(grouped.size());

    for (auto& [core, members] : grouped) {
        std::ranges::sort(members);
        mergedGroups.push_back(std::move(members));
    }

    return mergedGroups;
}

std::string FormatStateList(const std::vector<size_t>& states) {
    if (states.empty()) {
        return "[]";
    }

    std::string result = "[";
    for (size_t i = 0; i < states.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += std::format("I{}", states[i]);
    }
    result += "]";
    return result;
}

std::vector<LR1ItemSet> MergeLR1StatesByCore(const std::vector<LR1CanonicalState>& States) {
    const auto grouped = ComputeMergedStateGroups(States);

    std::vector<LR1ItemSet> mergedStates;
    mergedStates.reserve(grouped.size());

    for (const auto& members : grouped) {
        LR1ItemSet mergedState;
        for (const auto& memberIndex : members) {
            mergedState.insert(States[memberIndex].Items.begin(), States[memberIndex].Items.end());
        }
        mergedStates.push_back(mergedState);
    }

    return mergedStates;
}

void PrintMergedLR1States(const std::vector<LR1CanonicalState>& States) {
    const auto grouped = ComputeMergedStateGroups(States);

    Surab::Log("\nMerged LALR(1) cores:");
    for (size_t mergedIndex = 0; mergedIndex < grouped.size(); ++mergedIndex) {
        std::string printString = std::format("M[{}]: {}", mergedIndex, FormatStateList(grouped[mergedIndex]));
        Surab::Log("{}", printString);
    }
}

ActionTable_t BuildLALRActionTable(
    const GrammarClass& AugmentedGrammar,
    const std::vector<LR1ItemSet>& MergedStates,
    const std::vector<std::vector<size_t>>& MergedGroups,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const auto& GrammarList = AugmentedGrammar.GetSplitProductions();
    const Token_t StartSymbol = AugmentedGrammar.GetStartSymbol();
    const OrderedTokenSet NonTerminals = AugmentedGrammar.GetNonTerminals();

    std::map<std::string, size_t> coreToState;
    for (size_t i = 0; i < MergedStates.size(); ++i) {
        coreToState[CoreKey(MergedStates[i])] = i;
    }

    ActionTable_t actionTable;
    for (size_t stateId = 0; stateId < MergedStates.size(); ++stateId) {
        const LR1ItemSet& state = MergedStates[stateId];

        for (const auto& [prodIndex, dotIndex, lookAhead] : state) {
            const auto rule = GrammarList[prodIndex];
            const TokenizedProduction_t& production = rule.TokenizedProductionList[0];

            if (dotIndex == production.size()) {
                if (rule.ProductionSymbol == StartSymbol) {
                    actionTable[stateId]["$"] = "ACCEPT";
                }
                else {
                    actionTable[stateId][lookAhead] = std::format("reduce [{}]: {}", prodIndex, rule.ToString());
                }
            }
            else {
                const Token_t symbolAfterDot = production[dotIndex];
                if (!NonTerminals.contains(symbolAfterDot)) {
                    const LR1ItemSet shifted = GotoLR1(AugmentedGrammar, state, symbolAfterDot, FirstMap);
                    const auto it = coreToState.find(CoreKey(shifted));
                    if (it != coreToState.end()) {
                        const std::string targetLabel = FormatStateList(MergedGroups[it->second]);
                        actionTable[stateId][symbolAfterDot] = std::format("shift {} {}", it->second, targetLabel);
                    }
                }
            }
        }
    }

    return actionTable;
}

GotoTable_t BuildLALRGotoTable(
    const GrammarClass& AugmentedGrammar,
    const std::vector<LR1ItemSet>& MergedStates,
    const std::vector<std::vector<size_t>>& MergedGroups,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const OrderedTokenSet NonTerminals = AugmentedGrammar.GetNonTerminals();
    GotoTable_t gotoTable;

    std::map<std::string, size_t> coreToState;
    for (size_t i = 0; i < MergedStates.size(); ++i) {
        coreToState[CoreKey(MergedStates[i])] = i;
    }

    for (size_t stateId = 0; stateId < MergedStates.size(); ++stateId) {
        const LR1ItemSet& state = MergedStates[stateId];

        for (const auto& nt : NonTerminals) {
            const LR1ItemSet shifted = GotoLR1(AugmentedGrammar, state, nt, FirstMap);
            if (shifted.empty()) {
                continue;
            }

            const auto it = coreToState.find(CoreKey(shifted));
            if (it != coreToState.end()) {
                gotoTable[stateId][nt] = it->second;
            }
        }
    }

    return gotoTable;
}

int main() {
    Header("LALR(1) Parsing Table Builder");

    std::stringstream grammarString(R"(
        S -> AA
        A -> aA | b
    )");

    GrammarClass originalGrammar(grammarString, "S");
    GrammarClass augmentedGrammar = originalGrammar.GetAugmentedGrammar();

    Surab::Log("\nAugmented Grammar:");
    augmentedGrammar.PrintGrammar();

    Surab::Log("\nProduction List: ");
    augmentedGrammar.PrintProductionList();

    const auto firstMap = Surab::Compiler::Parser::ComputeFirst(augmentedGrammar);
    const auto followMap = Surab::Compiler::Parser::ComputeFollow(augmentedGrammar, firstMap);

    const auto states = CanonicalCollectionLR1(augmentedGrammar, firstMap);
    Surab::Compiler::LR1::PrintCanonicalStates(augmentedGrammar, states);

    PrintMergedLR1States(states);

    const auto mergedGroups = ComputeMergedStateGroups(states);
    const auto mergedStates = MergeLR1StatesByCore(states);
    ActionTable_t actionTable = BuildLALRActionTable(augmentedGrammar, mergedStates, mergedGroups, firstMap);
    GotoTable_t gotoTable = BuildLALRGotoTable(augmentedGrammar, mergedStates, mergedGroups, firstMap);

    PrintCombinedParseTable(augmentedGrammar, actionTable, gotoTable, mergedGroups);

    Footer();
    return 0;
}
