#pragma once

#include "src/commons.hpp"

#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace Surab
{
namespace Compiler
{
// Custom derived vector class with set like behavior for storing unique strings but preserving insertion order.
template<typename T>
class OrderedSet : public std::vector<T>
{
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    OrderedSet() = default;

    OrderedSet(const T& value) { this->push_back(value); }

    OrderedSet(T&& value) { this->push_back(std::move(value)); }
public:

    bool contains(const T& value) const { return std::ranges::contains(*this, value); }

    std::pair<iterator, bool> insert(const T& value) {
        bool contains = std::ranges::contains(*this, value);
        if (!contains) {
            this->push_back(value);
            return { this->end() - 1, true };
        }
        return { std::ranges::find(*this, value), false };
    }

    std::pair<iterator, bool> insert(const const_iterator& begin, const const_iterator& end) {
        bool changed = false;
        for (auto it = begin; it != end; ++it) {
            if (!std::ranges::contains(*this, *it)) {
                this->push_back(*it);
                changed = true;
            }
        }
        return { this->end() - 1, changed };
    }
};


using Token_t = std::string;
using OrderedTokenSet = OrderedSet<Token_t>;

constexpr Token_t EPSILON = "∈";
constexpr Token_t DOT = "•";

namespace Grammar
{
using TokenizedProduction_t = std::vector<Token_t>;
// untokenized production string
using RawProduction_t = std::string;
struct ProductionRule {
    Token_t ProductionSymbol;
    std::vector<TokenizedProduction_t> TokenizedProductionList;

    std::string ToString() const {
        std::string rhsString;
        for (const auto& production : TokenizedProductionList) {
            for (size_t i = 0; i < production.size(); ++i) {
                rhsString += production[i];
            }
            if (production != TokenizedProductionList.back())
                rhsString += " | ";
        }

        return ProductionSymbol + " -> " + rhsString;
    }

public:
    static std::vector<RawProduction_t> SplitProductions(const std::string& rhs) {
        std::vector<RawProduction_t> parts;
        std::stringstream ss(rhs);
        RawProduction_t item;
        while (std::getline(ss, item, '|')) {
            parts.push_back(Trim(item));
        }
        return parts;
    }
};

class GrammarClass {
public:
///////////////////
// Constructors //
///////////////////
    GrammarClass() = default;

    GrammarClass(std::stringstream& Input, Token_t StartSymbol = "S") {
        m_StartSymbol = StartSymbol;

        std::string lineBuffer;


        std::stringstream InputCopy(Input.str());
        OrderedTokenSet NonTerminals;

        // Collect the non Terminals
        while (std::getline(InputCopy, lineBuffer)) {
            auto [LHS, _] = ParseProductionStringLine(lineBuffer);
            NonTerminals.insert(LHS);
        }

        // Parse the grammar rules
        while (std::getline(Input, lineBuffer)) {
            auto [LHS, RHS] = ParseProductionStringLine(lineBuffer);

            for (const std::string& productionString : ProductionRule::SplitProductions(RHS)) {
                const TokenizedProduction_t production = GrammarClass::TokenizeProduction(productionString, NonTerminals);
                (*this)[LHS].push_back(production);
            }
        }

        InitializeCache();
    }


    GrammarClass GetAugmentedGrammar() const {
        GrammarClass augmented = *this;
        const Token_t augmentedStart = GetStartSymbol() + "'";

        ProductionRule augmentedProduction;
        augmentedProduction.ProductionSymbol = augmentedStart;
        augmentedProduction.TokenizedProductionList = { { GetStartSymbol() } };

        augmented.m_GrammarRules.insert(augmented.m_GrammarRules.begin(), augmentedProduction);

        augmented.m_StartSymbol = augmentedStart;
        augmented.InitializeCache();

        return augmented;
    }

public:
/////////////////////////
// Container Interface //
/////////////////////////
    size_t size() const { return m_GrammarRules.size(); }

    std::vector<ProductionRule>::const_iterator begin() const { return m_GrammarRules.begin(); }
    std::vector<ProductionRule>::const_iterator end() const { return m_GrammarRules.end(); }

    std::vector<TokenizedProduction_t>& operator[](const Token_t& lhs) {
        for (size_t i = 0; i < m_GrammarRules.size(); ++i) {
            if (m_GrammarRules[i].ProductionSymbol == lhs) {
                return m_GrammarRules[i].TokenizedProductionList;
            }
        }
        m_GrammarRules.push_back({ lhs, {} });
        return m_GrammarRules.back().TokenizedProductionList;
    }

    const std::vector<TokenizedProduction_t> at(const Token_t& lhs) const {
        for (const auto& [symbol, productions] : m_GrammarRules) {
            if (symbol == lhs) {
                return productions;
            }
        }
        throw std::out_of_range("Non-terminal not found in grammar: " + lhs);
    }

    auto insert(const Token_t& lhs, const std::vector<TokenizedProduction_t>& productions) {
        return m_GrammarRules.emplace_back(lhs, productions);
    }

    bool contains(const Token_t& lhs) const {
        return std::ranges::any_of(m_GrammarRules, [&](const ProductionRule& rule) { return rule.ProductionSymbol == lhs; });
    }

    std::vector<ProductionRule>::const_iterator erase(const Token_t& lhs) {
        auto it = std::ranges::find_if(m_GrammarRules, [&](const ProductionRule& rule) { return rule.ProductionSymbol == lhs; });
        if (it != m_GrammarRules.end()) {
            return m_GrammarRules.erase(it);
        }
        return m_GrammarRules.end();
    }

public:
/////////////////////////
//   Class Interface   //
/////////////////////////

    bool IsNonTerminal(const Token_t& Symbol) { return m_Cache.NonTerminals.contains(Symbol); }

    void PrintGrammar() const {
        for (const auto& prodRule : m_GrammarRules) {
            Surab::Log("{}", prodRule.ToString());
        }
    }

    void PrintProductionList() const {
        const std::vector<ProductionRule> productionList = GetSplitProductions();
        int i = 0;
        for (const auto& prodRule : productionList) {
            Surab::Log("P[{}]: {}", i, prodRule.ToString());
            ++i;
        }
    }

public:
    static std::pair<std::string, std::string> ParseProductionStringLine(const std::string& Line) {
        auto pos = Line.find("->");
        if (pos == std::string::npos) {
            return { "", "" };
        }

        std::string LHS = Trim(Line.substr(0, pos));
        std::string RHS = Trim(Line.substr(pos + 2));

        return { LHS, RHS };
    }
    static std::vector<Token_t> TokenizeProduction(const std::string& Production, const OrderedTokenSet& NonTerminals) {
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

    static std::string FormatTokens(const std::vector<Token_t>& tokens) {
        std::string result;
        for (const auto& token : tokens) {
            result += token;
        }
        return Trim(result);
    }

public:
    inline Token_t GetStartSymbol() const { return m_StartSymbol; }
    // splits every ProductionRule.RHS to single map of LHS -> RHS for each item in RHS
    inline std::vector<ProductionRule> GetSplitProductions() const { return m_Cache.SplitProductions; }
    inline OrderedTokenSet GetNonTerminals() const { return m_Cache.NonTerminals; }
    inline OrderedTokenSet GetTerminals() const { return m_Cache.Terminals; }
    inline std::pair<OrderedTokenSet, OrderedTokenSet> GetNonTerminalsAndTerminals() const { return { m_Cache.NonTerminals, m_Cache.Terminals }; }
protected:
    void InitializeCache() {
        std::vector<ProductionRule> splitProductions;
        // Calculate split productions
        {
            for (const auto& [lhs, rhs] : m_GrammarRules) {
                for (const auto& production : rhs) {
                    splitProductions.push_back({ lhs, { production } });
                }
            }
        }
        m_Cache.SplitProductions = splitProductions;

        OrderedTokenSet NT, T;
        // Calculate Non-Terminals first
        for (const auto& [A, productionList] : m_GrammarRules) {
            NT.insert(A);
        }

        // Non-Terminals are used in calculation of Terminals
        for (const auto& [A, productionList] : m_GrammarRules) {
            for (const TokenizedProduction_t& prod : productionList) {
                for (const Token_t& token : prod) {
                    if (token != EPSILON && !NT.contains(token))
                        T.insert(token);
                }
            }
        }

        m_Cache.NonTerminals = NT;
        m_Cache.Terminals = T;
    }

private:
    std::vector<ProductionRule> m_GrammarRules;
    Token_t m_StartSymbol;

    struct CachedData {
        std::vector<ProductionRule> SplitProductions;
        OrderedTokenSet NonTerminals;
        OrderedTokenSet Terminals;
    } m_Cache;
};


} // namespace Grammar


namespace Parser
{
using namespace Surab::Compiler::Grammar;

using ActionTable_t = std::unordered_map<size_t, std::unordered_map<Token_t, std::string>>;
using GotoTable_t = std::unordered_map< size_t, std::unordered_map<Token_t, size_t>>;


OrderedTokenSet FIRST(
    const GrammarClass& G,
    Token_t Token,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap
) {
    if (FirstResMap.contains(Token)) {
        return FirstResMap.at(Token);
    }

    const OrderedTokenSet& NT = G.GetNonTerminals();

    if (Token == EPSILON) {
        return { EPSILON };
    }

    if (NT.contains(Token)) {
        // X -> A | B | aplha | ∈
        // return FIRST(A) U FIRST(B) U FIRST(alpha) U {∈}
        OrderedTokenSet firstOfSymbol;

        for (const TokenizedProduction_t& production : G.at(Token)) {
            OrderedTokenSet firstOfProd = FIRST(G, production[0], FirstResMap);
            firstOfSymbol.insert(firstOfProd.begin(), firstOfProd.end());
        }
        return firstOfSymbol;
    }
    // Token is a Terminal Or Production
    TokenizedProduction_t tokenList = GrammarClass::TokenizeProduction(Token, NT);

    // If it is a single terminal, return it.
    if (tokenList.size() == 1 && !NT.contains(tokenList[0]))
        return { tokenList[0] };

    // FIRST of a production.
    OrderedTokenSet firstOfProduction;
    bool nullable = true;

    for (const Token_t& token : tokenList) {
        OrderedTokenSet firstOfSym = FIRST(G, token, FirstResMap);

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

std::unordered_map<Token_t, OrderedTokenSet> ComputeFirst(const GrammarClass& G) {
    const auto NT = G.GetNonTerminals();
    std::unordered_map<Token_t, OrderedTokenSet> firstResMap;
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto& [lhs, productions] : G) {
            for (const auto& production : productions) {
                const auto tokens = GrammarClass::TokenizeProduction(production[0], NT);
                bool nullable = true;
                for (const auto& token : tokens) {
                    if (token == EPSILON) continue;
                    const OrderedSet firstToken = NT.contains(token) ? firstResMap[token] : OrderedSet{ token };
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

OrderedTokenSet FOLLOW(
    const GrammarClass& G,
    const Token_t& Token,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap,
    const std::unordered_map<Token_t, OrderedTokenSet>& FollowResMap,
    const Token_t& InitiatingToken = ""
) {
    const OrderedTokenSet NT = G.GetNonTerminals();

    OrderedTokenSet followOfSymbol;

    if (FollowResMap.contains(Token)) {
        const auto& res = FollowResMap.at(Token);
        followOfSymbol.insert(res.cbegin(), res.cend());
    }

    for (const auto& [A, productionList] : G) {
        for (const TokenizedProduction_t& tokenList : productionList) {
            for (std::size_t i = 0; i < tokenList.size(); ++i) {
                if (tokenList[i] != Token)
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
                    const OrderedTokenSet& firstBeta = FirstResMap.at(beta);

                    for (const Token_t& x : firstBeta) {
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
                    A != Token &&
                    A != InitiatingToken // Prevent the stack from blowing up
                    ) {
                    OrderedSet followOfA = FOLLOW(G, A, FirstResMap, FollowResMap, Token);
                    followOfSymbol.insert(followOfA.begin(), followOfA.end());
                }
            }
        }
    }

    return followOfSymbol;
}

std::unordered_map<Token_t, OrderedTokenSet> ComputeFollow(
    const GrammarClass& G,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap
) {
    const OrderedTokenSet NT = G.GetNonTerminals();
    std::unordered_map<Token_t, OrderedTokenSet> followResMap;
    // Initialize FOLLOW of start symbol
    if (NT.contains(G.GetStartSymbol())) {
        followResMap[G.GetStartSymbol()].insert("$");
    }
    else {
        followResMap[*NT.begin()].insert("$");
    }

    for (const auto& X : NT)
        followResMap[X] = FOLLOW(G, X, FirstResMap, followResMap);

    return followResMap;
}

// Non-Terminal to Terminal to Production
using ParseTable_t = std::unordered_map<Token_t, std::unordered_map<Token_t, std::string>>;
ParseTable_t ConstructParseTable(
    const GrammarClass& G,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap,
    const std::unordered_map<Token_t, OrderedTokenSet>& FollowResMap
) {
    ParseTable_t table;
    for (const auto& [A, productionList] : G) {
        for (const TokenizedProduction_t& prod : productionList) {
            // FIRST of the production
            OrderedTokenSet firstp = FIRST(G, prod[0], FirstResMap);

            // For every terminal in FIRST(prod)
            for (const auto& a : firstp) {
                if (a != EPSILON) {
                    // If the cell is already occupied, there's a conflict
                    bool cellOccupied = table.contains(A) && table.at(A).contains(a) && !table.at(A).at(a).empty();
                    if (cellOccupied) {
                        LogError("Conflict in parse table for [{}, {}]", A, a);
                        break;
                    }
                    table[A][a] = A + " -> " + GrammarClass::FormatTokens(prod);
                }
            }

            // If the production can derive epsilon,
            // add it to every terminal in FOLLOW(A)
            if (firstp.contains(EPSILON)) {
                OrderedSet followA = FOLLOW(G, A, FirstResMap, FollowResMap);

                for (const auto& b : followA) {
                    bool cellOccupied = table.contains(A) && table.at(A).contains(b) && !table.at(A).at(b).empty();
                    if (cellOccupied) {
                        LogError("Conflict in parse table for [{}, {}]", A, b);
                        break;
                    }
                    table[A][b] = A + " -> " + GrammarClass::FormatTokens(prod);
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
    const std::unordered_map<RowKey_t, std::unordered_map<ColKey_t, Value_t>>& Table,
    const std::string& RowHeader = "State"
) {
    // Collect all row and column keys
    std::set<RowKey_t> RowKeys = Table
        | std::views::keys
        | std::ranges::to<std::set<RowKey_t>>();

    std::set<ColKey_t> ColumnKeys = Table
        | std::views::values
        | std::views::transform([](const std::unordered_map<ColKey_t, Value_t>& colMap) { return colMap | std::views::keys; })
        | std::views::join
        | std::ranges::to<std::set<ColKey_t>>();


    // Build the text for every cell
    std::unordered_map<RowKey_t, std::unordered_map<ColKey_t, std::string>> cells;

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
    std::unordered_map<ColKey_t, int> columnWidths;
    {
        for (const ColKey_t& columnKey : ColumnKeys) {
            auto columnCellValues = cells
                | std::views::values
                | std::views::filter([columnKey](const std::unordered_map<ColKey_t, std::string>& map) { return map.contains(columnKey); })
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
    const GrammarClass& Grammar,
    const ActionTable_t& ActionTable,
    const GotoTable_t& GotoTable
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

    // Sort based on state number
    std::sort(RowKeys.begin(), RowKeys.end(), [](const std::string& a, const std::string& b) {
        return std::stoi(a) < std::stoi(b);
        });
    RowKeys.insert(RowKeys.begin(), RowHeader);

    // Sort Terminals and NonTerminals in ColumnKeys based on their order in Grammar.GetTerminals() and Grammar.GetNonTerminals()
    {
        const auto& NonTerminals = Grammar.GetNonTerminals();
        const auto& Terminals = Grammar.GetTerminals();

        std::ranges::sort(ColumnKeys.begin(), ColumnKeys.begin() + actionTableColumnCount, [&](const auto& a, const auto& b) {
            return std::ranges::find(Terminals, a) < std::ranges::find(Terminals, b);
            });
        std::ranges::sort(ColumnKeys.begin() + actionTableColumnCount, ColumnKeys.end(), [&](const auto& a, const auto& b) {
            return std::ranges::find(NonTerminals, a) < std::ranges::find(NonTerminals, b);
            });
    }
    ColumnKeys.insert(ColumnKeys.begin(), RowHeader);


    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> cells;

    // Build cell values
    for (const auto& rowKey : RowKeys) {
        for (const auto& columnKey : ColumnKeys) {
            std::string cell;
            if (rowKey == RowHeader || columnKey == RowHeader) {
                cell = columnKey == RowHeader ? rowKey : columnKey;
            }
            else {
                if (Grammar.GetNonTerminals().contains(columnKey)) {
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
    std::unordered_map<std::string, int> columnWidths;
    {
        for (const std::string& columnKey : ColumnKeys) {
            auto columnCellValues = cells
                | std::views::values
                | std::views::filter([columnKey](const std::unordered_map<std::string, std::string>& map) { return map.contains(columnKey); })
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
using namespace Surab::Compiler::Grammar;
struct ParserItem {
    size_t ProductionIndex;
    size_t DotPosition;

    bool operator<(const ParserItem& other) const {
        return std::tie(ProductionIndex, DotPosition) < std::tie(other.ProductionIndex, other.DotPosition);
    }

    bool operator==(const ParserItem& other) const {
        return ProductionIndex == other.ProductionIndex && DotPosition == other.DotPosition;
    }
};

struct ParserCanonicalState {
    size_t StateId;
    size_t CalleeStateId;
    Token_t CalleeSymbol;
    std::set<ParserItem> Items;
};

std::string FormatItem(const GrammarClass& Grammar, const ParserItem& Item) {
    const auto& [prodIndex, dotIndex] = Item;
    const auto& productionRuleList = Grammar.GetSplitProductions();
    const auto& [lhs, rhs] = productionRuleList.at(prodIndex);

    // Split Productions have only one production in the rhs
    const TokenizedProduction_t& productionTokens = rhs[0];

    std::string result = lhs + " -> ";
    for (size_t i = 0; i < productionTokens.size(); ++i) {
        if (i == dotIndex)
            result += DOT;
        result += productionTokens[i] == EPSILON ? "" : productionTokens[i];
    }
    if (dotIndex == productionTokens.size())
        result += DOT;
    return result;
}

std::string StateKey(const GrammarClass& Grammar, const std::set<ParserItem>& State) {
    std::string key;
    for (const auto& item : State) {
        if (!key.empty())
            key += " | ";
        key += FormatItem(Grammar, item);
    }
    return key;
}

void PrintCanonicalStates(const GrammarClass& Grammar, const std::vector<ParserCanonicalState>& States) {
    Surab::Log("\nCanonical LR(0) collection states:");

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
} // namespace SLR


namespace LR1
{

using namespace Surab::Compiler::Grammar;
struct ParserItem {
    size_t ProductionIndex;
    size_t DotPosition;
    Token_t Lookahead;

    bool operator<(const ParserItem& other) const {
        return std::tie(ProductionIndex, DotPosition, Lookahead) < std::tie(other.ProductionIndex, other.DotPosition, other.Lookahead);
    }

    bool operator==(const ParserItem& other) const {
        return ProductionIndex == other.ProductionIndex && DotPosition == other.DotPosition && Lookahead == other.Lookahead;
    }
};

struct ParserCanonicalState {
    size_t StateId;
    size_t CalleeStateId;
    Token_t CalleeSymbol;
    std::set<ParserItem> Items;
};

std::string FormatItem(const GrammarClass& Grammar, const ParserItem& Item) {
    const auto& [prodIndex, dot, lookahead] = Item;
    std::string result = SLR::FormatItem(Grammar, { prodIndex, dot });
    result += ", " + lookahead;
    return result;
}

void PrintCanonicalStates(const GrammarClass& Grammar, const std::vector<ParserCanonicalState>& States) {
    Surab::Log("\nCanonical LR(1) states:");
    for (size_t i = 0; i < States.size(); ++i) {
        std::string printString = std::format("I[{}]: ", i);

        const auto& [StateId, CalleeStateId, CalleeSymbol, Items] = States[i];
        printString += std::format("{}({}, {}) = ", StateId == 0 ? "Closure" : "Goto", CalleeStateId, CalleeSymbol);

        printString += "[ ";
        for (const auto& item : Items) {
            printString += "{" + FormatItem(Grammar, item) + "} , ";
        }

        // Remove the last ", "
        printString.erase(printString.size() - 2, 2);
        printString += " ]";
        Surab::Log("{}", printString);
    }
}

std::string StateKey(const GrammarClass& Grammar, const std::set<ParserItem>& State) {
    std::string key;
    for (const auto& item : State) {
        if (!key.empty())
            key += " | ";
        key += "{" + FormatItem(Grammar, item) + "}";
    }
    return key;
}
} // namespace SLR1
} // namespace Compiler

} // namespace Surab

#define EPSILON Surab::Compiler::EPSILON
#define DOT Surab::Compiler::DOT