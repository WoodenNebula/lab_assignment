#pragma once

#include "src/commons.hpp"

#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

namespace Surab
{
namespace Compiler
{
using Token_t = std::string;
using Production_t = Token_t;
using Grammar_t = std::map<Token_t, std::vector<Production_t>>;
using SSet_t = std::set<std::string>;
using GrammarList_t = std::vector<std::pair<Token_t, std::vector<Token_t>>>;

constexpr Token_t EPSILON = "∈";
constexpr std::string DOT = "•";

namespace Grammar
{
std::vector<Production_t> SplitProductions(const std::string& rhs) {
    std::vector<Production_t> parts;
    std::stringstream ss(rhs);
    std::string item;
    while (std::getline(ss, item, '|')) {
        parts.push_back(Trim(item));
    }
    return parts;
}

Grammar_t ParseGrammarFromString(std::stringstream& Input) {
    Grammar_t G;
    std::string lineBuffer;
    while (std::getline(Input, lineBuffer)) {
        // Find the non-terminal
        auto pos = lineBuffer.find("->");
        if (pos == std::string::npos)
            continue;

        std::string LHS = Trim(lineBuffer.substr(0, pos));
        std::string RHS = Trim(lineBuffer.substr(pos + 2));

        for (const Production_t& p : SplitProductions(RHS))
            G[LHS].push_back(p);
    }
    return G;
}

std::vector<Token_t> TokenizeProduction(const Production_t& Production, const SSet_t& NonTerminals) {
    std::vector<Token_t> Tokens;

    for (std::size_t i = 0; i < Production.size();) {
        // Skip whitespace
        if (std::isspace(Production[i])) {
            ++i;
            continue;
        }

        if (Production == EPSILON) {
            Tokens.emplace_back(EPSILON);
            i++;
            break;
        }

        // Try to match a non-terminal.
        // Longest match first.
        std::string matchedNT;

        for (const Token_t& nt : NonTerminals) {
            bool isMatch = Production.compare(i, nt.size(), nt) == 0;
            bool isLongerMatch = nt.size() > matchedNT.size();
            if (isMatch && isLongerMatch) {
                matchedNT = nt;
            }
        }

        if (!matchedNT.empty()) {
            Tokens.push_back(matchedNT);
            i += matchedNT.size();
            continue;
        }

        // Otherwise consume a terminal token.
        //
        // For now, terminals are sequences of
        // alphanumeric lower characters.
        auto isSmallCaseLetter = [](char c) {return c >= 'a' && c <= 'z';};

        if (std::isalnum(Production.at(i)) && isSmallCaseLetter(Production.at(i))) {
            Token_t terminal;

            while (i < Production.size() &&
                std::isalnum(Production.at(i)) &&
                isSmallCaseLetter(Production.at(i))) {
                terminal += Production.at(i++);
            }

            Tokens.push_back(terminal);
            continue;
        }

        // Single-character terminal such as:
        // (, ), +, *, etc.
        Tokens.emplace_back(1, Production.at(i));
        ++i;
    }

    return Tokens;
}

std::pair<SSet_t, SSet_t> IdentifyNonTerminalsAndTerminals(const Grammar_t& G) {
    SSet_t NT;
    SSet_t T;

    // First collect all non-terminals.
    for (const auto& [A, _] : G)
        NT.insert(A);

    // Then tokenize productions.
    for (const auto& [A, productionList] : G) {
        for (const Production_t& prod : productionList) {
            std::vector<Token_t> tokenList = TokenizeProduction(prod, NT);

            for (const Token_t& token : tokenList) {
                if (token != EPSILON && !NT.contains(token))
                    T.insert(token);
            }
        }
    }

    return { NT, T };
}

bool IsNonTerminal(const Token_t& symbol, const SSet_t& nonTerminals) { return nonTerminals.contains(symbol); }

Grammar_t AugmentGrammar(const Grammar_t& G, const Token_t& StartSymbol) {
    Grammar_t augmented = G;
    const Token_t augmentedStart = StartSymbol + "'";
    augmented.insert(augmented.begin(), { augmentedStart, { StartSymbol } });
    return augmented;
}

GrammarList_t ToProductionVector(const Grammar_t& G, const Token_t& StartSymbol = "S") {
    const auto [nonTerminals, terminals] = IdentifyNonTerminalsAndTerminals(G);
    GrammarList_t rules;
    for (const auto& [lhs, productions] : G) {
        for (const auto& production : productions) {
            if (lhs == StartSymbol + "'" || lhs == StartSymbol) {
                // skip start and augmented start to add them at the beginning
                continue;
            }
            rules.push_back({ lhs, TokenizeProduction(production, nonTerminals) });
        }
    }

    auto prodsOfStart = G.at(StartSymbol);
    for (const auto& production : prodsOfStart) {
        rules.insert(rules.begin(), { StartSymbol, TokenizeProduction(production, nonTerminals) });
    }

    auto prodsOfAugmentedStart = G.at(StartSymbol + "'");
    for (const auto& production : prodsOfAugmentedStart) {
        rules.insert(rules.begin(), { StartSymbol + "'", TokenizeProduction(production, nonTerminals) });
    }
    return rules;
}


////////////////////////
// Printing functions //
////////////////////////
std::string FormatProduction(const Token_t& lhs, const Production_t& rhs) {
    return lhs + " -> " + rhs;
}

std::string FormatProduction(const std::pair<Token_t, std::vector<Token_t>>& rule) {
    std::string rhs;
    for (const auto& token : rule.second) {
        rhs += token;
    }
    return FormatProduction(rule.first, rhs);
}

void PrintGrammar(const Grammar_t& G) {
    for (const auto& [A, prods] : G) {
        std::string rhs;
        for (size_t i = 0; i < prods.size(); ++i) {
            rhs += prods[i];
            if (i != prods.size() - 1) rhs += " | ";
        }
        std::println("{} -> {}", A, rhs);
    }
}

void PrintGrammarList(const GrammarList_t& grammarList) {
    for (size_t i = 0; i < grammarList.size(); ++i) {
        const auto& [lhs, rhs] = grammarList[i];
        std::string rhsStr;
        for (const auto& token : rhs) {
            rhsStr += token + " ";
        }
        Surab::Log("P[{}]: {} -> {}", i, lhs, rhsStr);
    }
}
} // namespace Grammar

using namespace Grammar;

namespace Parser
{
using ActionTable_t = std::map<size_t, std::map<Token_t, std::string>>;
using GotoTable_t = std::map< size_t, std::map<Token_t, size_t>>;

SSet_t FIRST(
    Token_t TokenInstance,
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap
) {
    if (FirstResMap.contains(TokenInstance)) {
        return FirstResMap.at(TokenInstance);
    }

    auto [NT, T] = IdentifyNonTerminalsAndTerminals(G);

    if (TokenInstance == EPSILON) {
        return { EPSILON };
    }

    if (NT.contains(TokenInstance)) {
        // X -> A | B | aplha | ∈
        // return FIRST(A) U FIRST(B) U FIRST(alpha) U {∈}
        SSet_t firstOfSymbol;

        for (const auto& production : G.at(TokenInstance)) {
            SSet_t firstOfProd = FIRST(production, G, FirstResMap);
            firstOfSymbol.insert(firstOfProd.begin(), firstOfProd.end());
        }
        return firstOfSymbol;

    }
    // TokenInstance is a Terminal Or Production
    // Get list of tokens from the TokenInstance
    std::vector<Token_t> tokenList = TokenizeProduction(TokenInstance, NT);

    // If it is a single terminal, return it.
    if (tokenList.size() == 1 && !NT.contains(tokenList[0]))
        return { tokenList[0] };

    // FIRST of a production.
    SSet_t firstOfProduction;
    bool nullable = true;

    for (const Token_t& token : tokenList) {
        std::set<Token_t> firstOfSym = FIRST(token, G, FirstResMap);

        for (const Token_t& x : firstOfSym) {
            if (x != EPSILON)
                firstOfProduction.insert(x);
        }

        if (!firstOfSym.contains(EPSILON)) {
            nullable = false;
            break;
        }
    }

    if (nullable)
        firstOfProduction.insert(EPSILON);

    return firstOfProduction;

}

std::map<Token_t, SSet_t> ComputeFirst(const Grammar_t& G) {
    const auto [NT, _] = IdentifyNonTerminalsAndTerminals(G);
    std::map<Token_t, SSet_t> firstResMap;
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto& [lhs, productions] : G) {
            for (const auto& production : productions) {
                const auto tokens = TokenizeProduction(production, NT);
                bool nullable = true;
                for (const auto& token : tokens) {
                    if (token == EPSILON) continue;
                    const SSet_t firstToken = NT.contains(token) ? firstResMap[token] : SSet_t{ token };
                    for (const auto& value : firstToken) {
                        if (value != EPSILON && firstResMap[lhs].insert(value).second)
                            changed = true;
                    }
                    if (!firstToken.contains(EPSILON)) {
                        nullable = false;
                        break;
                    }
                }
                if (nullable && firstResMap[lhs].insert(EPSILON).second)
                    changed = true;
            }
        }
    }

    return firstResMap;
}

SSet_t FOLLOW(
    const Token_t& TokenInstance,
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap,
    const std::map<Token_t, SSet_t>& FollowResMap,
    const Token_t& InitiatingToken = ""
) {
    auto [NT, T] = IdentifyNonTerminalsAndTerminals(G);
    SSet_t followOfSymbol;

    if (FollowResMap.contains(TokenInstance)) {
        followOfSymbol.insert_range(FollowResMap.at(TokenInstance));
    }

    for (const auto& [A, productionList] : G) {
        for (const auto& prod : productionList) {
            std::vector<Token_t> tokenList = TokenizeProduction(prod, NT);

            for (std::size_t i = 0; i < tokenList.size(); ++i) {
                if (tokenList[i] != TokenInstance)
                    continue;

                bool nullableSuffix = true;

                // Examine everything after symbol.
                for (std::size_t j = i + 1; j < tokenList.size(); ++j) {
                    const Token_t& beta = tokenList[j];

                    // Terminal
                    if (!NT.contains(beta)) {
                        if (beta != EPSILON)
                            followOfSymbol.insert(beta);

                        nullableSuffix = false;
                        break;
                    }

                    // Non-terminal
                    const std::set<Token_t>& firstBeta = FirstResMap.at(beta);

                    for (const auto& x : firstBeta) {
                        if (x != EPSILON)
                            followOfSymbol.insert(x);
                    }

                    if (!firstBeta.contains(EPSILON)) {
                        nullableSuffix = false;
                        break;
                    }
                }

                // A -> α symbol
                // or A -> α symbol β where β => ε
                if (nullableSuffix &&
                    A != TokenInstance &&
                    A != InitiatingToken // Prevent the stack from blowing up
                    ) {
                    SSet_t followOfA = FOLLOW(A, G, FirstResMap, FollowResMap, TokenInstance);
                    followOfSymbol.insert(followOfA.begin(), followOfA.end());
                }
            }
        }
    }

    return followOfSymbol;
}

std::map<Token_t, SSet_t> ComputeFollow(
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap,
    const Token_t& StartSymbol = "S"
) {
    auto [NT, T] = IdentifyNonTerminalsAndTerminals(G);
    std::map<Token_t, SSet_t> followResMap;
    // Initialize FOLLOW of start symbol
    if (NT.contains(StartSymbol)) {
        followResMap[StartSymbol].insert("$");
    }
    else {
        followResMap[*NT.begin()].insert("$");
    }

    for (const auto& X : NT)
        followResMap[X] = FOLLOW(X, G, FirstResMap, followResMap);

    return followResMap;
}

// Non-Terminal to Terminal to Production
using ParseTable_t = std::map<Token_t, std::map<Token_t, Production_t>>;
ParseTable_t ConstructParseTable(
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap,
    const std::map<Token_t, SSet_t>& FollowResMap
) {
    ParseTable_t table;
    for (const auto& [A, productionList] : G) {
        for (const auto& prod : productionList) {
            // FIRST of the production
            SSet_t firstp = FIRST(prod, G, FirstResMap);

            // For every terminal in FIRST(prod)
            for (const auto& a : firstp) {
                if (a != EPSILON) {
                    // If the cell is already occupied, there's a conflict
                    bool cellOccupied = table.contains(A) && table.at(A).contains(a) && !table.at(A).at(a).empty();
                    if (cellOccupied) {
                        LogError("Conflict in parse table for [{}, {}]", A, a);
                        break;
                    }
                    table[A][a] = prod;
                }
            }

            // If the production can derive epsilon,
            // add it to every terminal in FOLLOW(A)
            if (firstp.contains(EPSILON)) {
                SSet_t followA = FOLLOW(A, G, FirstResMap, FollowResMap);

                for (const auto& b : followA) {
                    bool cellOccupied = table.contains(A) && table.at(A).contains(b) && !table.at(A).at(b).empty();
                    if (cellOccupied) {
                        LogError("Conflict in parse table for [{}, {}]", A, b);
                        break;
                    }
                    table[A][b] = prod;
                }
            }
        }
    }
    return table;
}


////////////////////////
// Printing functions //
////////////////////////

constexpr int COL_WIDTH = 20;
constexpr int ACTION_WIDTH = 30;
void PrintParsingStep(
    const std::string& stack,
    const std::string& input,
    const std::string& action,
    const std::string& ColorCode = RESET
) {
    Surab::Log("| {:<{}} | {:<{}} | {}{:<{}}{} |", stack, COL_WIDTH, input, COL_WIDTH, ColorCode, action, ACTION_WIDTH, RESET);
}
void PrintParsingSeparator() {
    Surab::Log("|{:-<{}}|{:-<{}}|{:-<{}}|", "", COL_WIDTH + 2, "", COL_WIDTH + 2, "", ACTION_WIDTH + 2);
}

template <typename RowKey_t, typename ColKey_t, typename Value_t>
void PrintParseTable(
    const std::map<RowKey_t, std::map<ColKey_t, Value_t>>& Table,
    const std::string& RowHeader = "State"
) {
    // Collect all row and column keys
    std::set<RowKey_t> RowKeys = Table
        | std::views::keys
        | std::ranges::to<std::set<RowKey_t>>();

    std::set<ColKey_t> ColumnKeys = Table
        | std::views::values
        | std::views::transform([](const std::map<ColKey_t, Value_t>& colMap) { return colMap | std::views::keys; })
        | std::views::join
        | std::ranges::to<std::set<ColKey_t>>();


    // Build the text for every cell
    std::map<RowKey_t, std::map<ColKey_t, std::string>> cells;

    for (const auto& rowKey : RowKeys) {
        for (const auto& columnKey : ColumnKeys) {
            if (!Table.contains(rowKey) || !Table.at(rowKey).contains(columnKey)) {
                cells[rowKey][columnKey] = "-";
                continue;
            }

            cells[rowKey][columnKey] = std::format("{}", Table.at(rowKey).at(columnKey));
        }
    }

    auto funcGetLengthAsString = [](const auto& value) {
        return std::format("{}", value).size();
        };

    // Calculate row-header width
    int rowWidth = std::ranges::max(RowKeys | std::views::transform(funcGetLengthAsString));
    rowWidth = std::max(rowWidth, (int)RowHeader.size());
    rowWidth += 2;

    // Calculate individual column widths
    std::map<ColKey_t, int> columnWidths;
    {
        for (const ColKey_t& columnKey : ColumnKeys) {
            auto columnCellValues = cells
                | std::views::values
                | std::views::filter([columnKey](const std::map<ColKey_t, std::string>& map) { return map.contains(columnKey); })
                | std::views::join;

            int width = std::ranges::max(columnCellValues | std::views::transform(funcGetLengthAsString));
            columnWidths[columnKey] = width + 2;
        }
    }

    // Separator
    auto funcPrintSeparator = [&]() {
        std::print("+{:-<{}}", "", rowWidth);

        for (const auto& columnKey : ColumnKeys) {
            std::print("+{:-<{}}", "", columnWidths.at(columnKey));
        }

        std::println("+");
        };


    funcPrintSeparator();
    // Header
    {
        std::print("|{:^{}}", RowHeader, rowWidth);
        for (const auto& columnKey : ColumnKeys) {
            std::print("|{:^{}}", std::format("{}", columnKey), columnWidths.at(columnKey)
            );
        }
        std::println("|");
    }
    funcPrintSeparator();


    // Rows
    {
        for (const auto& rowKey : RowKeys) {
            std::print("|{:^{}}", std::format("{}", rowKey), rowWidth);

            for (const auto& columnKey : ColumnKeys) {
                const auto& cell = cells.at(rowKey).at(columnKey);

                std::print("|{:^{}}", cell, columnWidths.at(columnKey));
            }

            std::println("|");
        }
    }
    funcPrintSeparator();
}


void PrintCombinedParseTable(
    const ActionTable_t& ActionTable,
    const GotoTable_t& GotoTable,
    const SSet_t& NonTerminals,
    const SSet_t& Terminals
) {
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


} // namespace Parser


namespace SLR
{
struct SItem {
    size_t ProductionIndex;
    size_t DotPosition;

    bool operator<(const SItem& other) const {
        return std::tie(ProductionIndex, DotPosition) < std::tie(other.ProductionIndex, other.DotPosition);
    }

    bool operator==(const SItem& other) const {
        return ProductionIndex == other.ProductionIndex && DotPosition == other.DotPosition;
    }
};

using SItemSet = std::set<SItem>;

struct SCanonicalState {
    size_t StateId;
    size_t CalleeStateId;
    Token_t CalleeSymbol;
    SItemSet Items;
};

std::string FormatItem(const GrammarList_t& grammar, const SItem& item) {
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

std::string StateKey(const GrammarList_t& GrammarList, const SItemSet& State) {
    std::string key;
    for (const auto& item : State) {
        if (!key.empty())
            key += " | ";
        key += FormatItem(GrammarList, item);
    }
    return key;
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
} // namespace SLR


namespace LR1
{
struct SItem {
    size_t ProductionIndex;
    size_t DotPosition;
    Token_t Lookahead;

    bool operator<(const SItem& other) const {
        return std::tie(ProductionIndex, DotPosition, Lookahead) < std::tie(other.ProductionIndex, other.DotPosition, other.Lookahead);
    }

    bool operator==(const SItem& other) const {
        return ProductionIndex == other.ProductionIndex && DotPosition == other.DotPosition && Lookahead == other.Lookahead;
    }
};

using LR1ItemSet_t = std::set<SItem>;

struct SCanonicalState {
    size_t StateId;
    size_t CalleeStateId;
    Token_t CalleeSymbol;
    LR1ItemSet_t Items;
};

std::string FormatItem(const GrammarList_t& Grammar, const SItem& Item) {
    const auto& [prodIndex, dot, lookahead] = Item;
    std::string result = SLR::FormatItem(Grammar, { prodIndex, dot });
    result += ", " + lookahead;
    return result;
}

void PrintCanonicalStates(const GrammarList_t& Grammar, const std::vector<SCanonicalState>& States) {
    Surab::Log("\nCanonical LR(1) states:");
    for (size_t i = 0; i < States.size(); ++i) {
        std::string printString = std::format("I[{}]: ", i);

        const auto& [StateId, CalleeStateId, CalleeSymbol, Items] = States[i];
        printString += std::format("{}({}, {}) = ", StateId == 0 ? "Closure" : "Goto", CalleeStateId, CalleeSymbol);

        printString += "[ ";
        for (const auto& item : Items) {
            printString += FormatItem(Grammar, item) + ", ";
        }

        // Remove the last ", "
        printString.erase(printString.size() - 2, 2);
        printString += " ]";
        Surab::Log("{}", printString);
    }
}


std::string StateKey(const GrammarList_t& grammar, const LR1ItemSet_t& state) {
    std::string key;
    for (const auto& item : state) {
        if (!key.empty()) key += " | ";
        key += FormatItem(grammar, item);
    }
    return key;
}
} // namespace SLR1
} // namespace Compiler


}

#define EPSILON Surab::Compiler::EPSILON
#define DOT Surab::Compiler::DOT