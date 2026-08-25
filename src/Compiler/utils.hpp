#pragma once

#include "src/commons.hpp"

#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <map>

namespace Surab
{
namespace Compiler
{
std::string Trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    return s;
}

using Token_t = std::string;
using Production_t = Token_t;
using Grammar_t = std::map<Token_t, std::vector<Production_t>>;
using Grammar_t = std::map<Token_t, std::vector<Production_t>>;
using SSet_t = std::set<std::string>;
using GrammarList_t = std::vector<std::pair<Token_t, std::vector<Token_t>>>;

constexpr Token_t EPSILON = "∈";

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

Grammar_t AugmentGrammar(const Grammar_t& G, const Token_t& StartSymbol) {
    Grammar_t augmented = G;
    const Token_t augmentedStart = StartSymbol + "'";
    augmented.insert(augmented.begin(), { augmentedStart, { StartSymbol } });
    return augmented;
}

GrammarList_t ToProductionVector(const Grammar_t& G) {
    const auto [nonTerminals, terminals] = IdentifyNonTerminalsAndTerminals(G);
    GrammarList_t rules;
    for (const auto& [lhs, productions] : G) {
        for (const auto& production : productions) {
            rules.push_back({ lhs, TokenizeProduction(production, nonTerminals) });
        }
    }
    return rules;
}

std::string FormatProduction(const Token_t& lhs, const Production_t& rhs) {
    return lhs + " -> " + rhs;
}

std::string FormatProduction(const std::pair<Token_t, std::vector<Token_t>>& rule) {
    std::string rhs;
    for (const auto& token : rule.second) {
        if (!rhs.empty()) rhs += " ";
        rhs += token;
    }
    return FormatProduction(rule.first, rhs);
}

std::string C2S(char c) { return std::string(1, c); }

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

using NT_t = Token_t;
using T_t = Token_t;
using ParseTable_t = std::map<NT_t, std::map<T_t, Production_t>>;

void PrintParseTable(const ParseTable_t& table) {
    // Build column and row data from the table
    std::set<Token_t> NonTerminals;
    std::set<Token_t> Terminals;

    for (const auto& [NT, T_map] : table) {
        NonTerminals.insert(NT);
        for (const auto& [T, _] : T_map) {
            Terminals.insert(T);
        }
    }

    // Build the text for every cell first.
    std::map<std::pair<NT_t, T_t>, std::string> cells;

    for (const auto& A : NonTerminals) {
        for (const auto& a : Terminals) {
            if (!table.contains(A)) {
                cells[{  A, a }] = "-";
                continue;
            }

            bool hasProduction = table.at(A).contains(a);
            bool isEmptyProduction = hasProduction && table.at(A).at(a).empty();

            if (!hasProduction || isEmptyProduction) {
                cells[{ A, a }] = "-";
                continue;
            }

            Production_t production = table.at(A).at(a);
            cells[{ A, a }] = production;
        }
    }

    // Calculate column widths.
    int ntWidth = 5;

    int max_width = 0;
    for (const auto& [table_key, val_map] : table) {
        for (const auto& [terminal, production] : val_map) {
            int width = production.size();
            max_width = std::max(max_width, width);
        }

    }
    max_width += 3;

    // Separator
    auto printSeparator = [&]() -> void {
        std::print("{}", "+" + std::string(ntWidth, '-'));
        for (const auto& terminal : Terminals) {
            std::print("{}", "+" + std::string(max_width, '-'));
        }
        std::println("{}", "+");
        };

    printSeparator();

    // Header
    std::print("|{:^{}}", "NT", ntWidth);

    for (const auto& terminal : Terminals) {
        std::print("|{:^{}}", terminal, max_width);
    }

    std::println("|");

    printSeparator();

    // Rows
    for (const auto& A : NonTerminals) {
        std::print("|{:^{}}", A, ntWidth);

        for (const auto& terminal : Terminals) {
            const auto& cell = cells[{A, terminal}];

            std::print("|{:^{}}", cell, max_width);
        }

        std::println("|");
    }

    printSeparator();
}


constexpr int COL_WIDTH = 20;
constexpr int ACTION_WIDTH = 30;
void PrintParsingStep(const std::string& stack, const std::string& input, const std::string& action, const std::string& ColorCode = RESET) {
    Surab::Log("| {:<{}} | {:<{}} | {}{:<{}}{} |", stack, COL_WIDTH, input, COL_WIDTH, ColorCode, action, ACTION_WIDTH, RESET);
}
void PrintParsingSeparator() {
    Surab::Log("|{:-<{}}|{:-<{}}|{:-<{}}|", "", COL_WIDTH + 2, "", COL_WIDTH + 2, "", ACTION_WIDTH + 2);
}


ParseTable_t ConstructParseTable(
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap,
    const std::map<Token_t, SSet_t>& FollowResMap
) {
    ParseTable_t table;
    for (const auto& [A, productionList] : G) {
        for (const auto& prod : productionList) {
            // FIRST of the production
            SSet_t firstp = Surab::Compiler::FIRST(prod, G, FirstResMap);

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
                SSet_t followA = Surab::Compiler::FOLLOW(A, G, FirstResMap, FollowResMap);

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

} // namespace Compiler
}

#define EPSILON Surab::Compiler::EPSILON