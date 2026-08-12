#include "src/commons.hpp"

#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <unordered_map>
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

using ProdList = std::vector<std::string>;
using Grammar = std::unordered_map<std::string, ProdList>;
using SSet = std::set<std::string>;
#define EPSILON "∈"

ProdList SplitProductions(const std::string& rhs) {
    ProdList parts;
    std::stringstream ss(rhs);
    std::string item;
    while (std::getline(ss, item, '|')) {
        parts.push_back(Trim(item));
    }
    return parts;
}

Grammar ParseGrammarFromString(std::stringstream& Input) {
    Grammar G;
    std::string lineBuffer;
    while (std::getline(Input, lineBuffer)) {
        // Find the non-terminal
        auto pos = lineBuffer.find("->");
        if (pos == std::string::npos)
            continue;

        std::string LHS = Trim(lineBuffer.substr(0, pos));
        std::string RHS = Trim(lineBuffer.substr(pos + 2));

        auto parts = SplitProductions(RHS);
        for (auto& p : parts) G[LHS].push_back(p);
    }
    return G;
}

std::vector<std::string> TokenizeProduction(const std::string& Production, const SSet& NonTerminals) {
    std::vector<std::string> Tokens;

    for (std::size_t i = 0; i < Production.size();) {
        // Skip whitespace
        if (std::isspace(Production[i])) {
            ++i;
            continue;
        }

        if (Production == EPSILON) {
            Tokens.emplace_back(EPSILON);
            i++;
            continue;
        }

        // Try to match a non-terminal.
        // Longest match first.
        std::string matchedNT;

        for (const auto& nt : NonTerminals) {
            if (Production.compare(i, nt.size(), nt) == 0 &&
                nt.size() > matchedNT.size()) {
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
        // alphanumeric characters.
        if (std::isalnum(static_cast<unsigned char>(Production[i]))) {
            std::string terminal;

            while (i < Production.size() &&
                std::isalnum(static_cast<unsigned char>(Production[i]))) {
                terminal += Production[i++];
            }

            Tokens.push_back(terminal);
            continue;
        }

        // Single-character terminal such as:
        // (, ), +, *, etc.
        Tokens.emplace_back(1, Production[i]);
        ++i;
    }

    return Tokens;
}



std::pair<SSet, SSet> IdentifyNonTerminalsAndTerminals(const Grammar& G) {
    SSet NT;
    SSet T;

    // First collect all non-terminals.
    for (const auto& [A, prods] : G)
        NT.insert(A);

    // Then tokenize productions.
    for (const auto& [A, prods] : G) {
        for (const auto& prod : prods) {
            auto symbols = TokenizeProduction(prod, NT);

            for (const auto& symbol : symbols) {
                if (symbol != EPSILON && !NT.contains(symbol))
                    T.insert(symbol);
            }
        }
    }

    return { NT, T };
}

void PrintGrammar(const Grammar& G) {
    for (const auto& [A, prods] : G) {
        std::string rhs;
        for (size_t i = 0; i < prods.size(); ++i) {
            rhs += prods[i];
            if (i != prods.size() - 1) rhs += " | ";
        }
        std::println("{} -> {}", A, rhs);
    }
}



std::string C2S(char c) { return std::string(1, c); };

SSet FIRST(std::string symbol, const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST_LIST) {
    if (FIRST_LIST.contains(symbol)) {
        return FIRST_LIST.at(symbol);
    }

    auto [NT, T] = IdentifyNonTerminalsAndTerminals(G);

    if (symbol == EPSILON) {
        return { EPSILON };
    }

    if (NT.contains(symbol)) {
        // X -> A | B | aplha | ∈
        // return FIRST(A) U FIRST(B) U FIRST(alpha) U {∈}
        SSet firstOfSymbol;

        for (const auto& prod : G.at(symbol)) {
            SSet firstOfProd = FIRST(prod, G, FIRST_LIST);
            firstOfSymbol.insert(firstOfProd.begin(), firstOfProd.end());
        }
        return firstOfSymbol;

    }
    // Symbol is a production or terminal
    auto symbols = TokenizeProduction(symbol, NT);

    // If it is a single terminal, return it.
    if (symbols.size() == 1 && !NT.contains(symbols[0]))
        return { symbols[0] };

    // FIRST of a production.
    SSet firstOfProduction;
    bool nullable = true;

    for (const auto& sym : symbols) {
        auto firstOfSym = FIRST(sym, G, FIRST_LIST);

        for (const auto& x : firstOfSym) {
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

std::unordered_map<std::string, SSet> ComputeFirst(const Grammar& G) {
    SSet NT;
    for (const auto& [A, prods] : G) {
        NT.insert(A);
    }

    std::unordered_map<std::string, SSet> FIRST_LIST;

    for (const auto& X : NT)
        FIRST_LIST[X] = FIRST(X, G, FIRST_LIST);

    return FIRST_LIST;
}

SSet FOLLOW(std::string symbol, const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST, const std::unordered_map<std::string, SSet>& FOLLOW_LIST) {
    auto [NT, T] = IdentifyNonTerminalsAndTerminals(G);
    SSet followOfSymbol;

    for (const auto& [A, prods] : G) {
        for (const auto& prod : prods) {
            auto symbols = TokenizeProduction(prod, NT);

            for (std::size_t i = 0; i < symbols.size(); ++i) {
                if (symbols[i] != symbol)
                    continue;

                bool nullableSuffix = true;

                // Examine everything after symbol.
                for (std::size_t j = i + 1; j < symbols.size(); ++j) {
                    const auto& beta = symbols[j];

                    // Terminal
                    if (!NT.contains(beta)) {
                        if (beta != EPSILON)
                            followOfSymbol.insert(beta);

                        nullableSuffix = false;
                        break;
                    }

                    // Non-terminal
                    const auto& firstBeta = FIRST.at(beta);

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
                if (nullableSuffix && A != symbol) {
                    SSet followOfA = FOLLOW(A, G, FIRST, FOLLOW_LIST);
                    followOfSymbol.insert(followOfA.begin(), followOfA.end());
                }
            }
        }
    }

    return followOfSymbol;
}


std::unordered_map<std::string, SSet> ComputeFollow(const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST, std::string_view startSymbol = "S") {
    auto [NT, T] = IdentifyNonTerminalsAndTerminals(G);
    std::unordered_map<std::string, SSet> FOLLOW_LIST;
    // Initialize FOLLOW of start symbol
    if (NT.contains(std::string(startSymbol))) {
        FOLLOW_LIST[std::string(startSymbol)].insert("$");
    }
    else {
        FOLLOW_LIST[*NT.begin()].insert("$");
    }

    for (const auto& X : NT)
        FOLLOW_LIST[X] = FOLLOW(X, G, FIRST, FOLLOW_LIST);

    return FOLLOW_LIST;
}

using NT_t = std::string;
using T_t = std::string;
using ParseTable = std::unordered_map<NT_t, std::unordered_map<T_t, ProdList>>;

void PrintTable(const ParseTable& table) {
    // Build column and row data from the table
    std::set<std::string> NonTerminals;
    std::set<std::string> Terminals;

    for (const auto& [NT, T_map] : table) {
        NonTerminals.insert(NT);
        for (const auto& [T, _] : T_map) {
            Terminals.insert(T);
        }
    }

    // Build the text for every cell first.
    std::map<std::pair<std::string, std::string>, std::string> cells;

    for (const auto& A : NonTerminals) {
        for (const auto& a : Terminals) {
            auto ntIt = table.find(A);

            if (ntIt == table.end()) {
                cells[{A, a}] = "-";
                continue;
            }

            auto tIt = ntIt->second.find(a);

            if (tIt == ntIt->second.end() || tIt->second.empty()) {
                cells[{A, a}] = "-";
                continue;
            }

            std::string cell;

            for (std::size_t i = 0; i < tIt->second.size(); ++i) {
                cell += tIt->second[i];

                if (i + 1 < tIt->second.size())
                    cell += " | ";
            }

            cells[{A, a}] = cell;
        }
    }

    // Calculate column widths.
    int ntWidth = 5;

    int max_width = 0;
    for (const auto& [table_key, val_map] : table) {
        for (const auto& [terminal, productions] : val_map) {
            for (const auto& production : productions) {
                int width = production.size();
                max_width = std::max(max_width, width);
            }
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

ParseTable ConstructParseTable(const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST, const std::unordered_map<std::string, SSet>& FOLLOW) {
    ParseTable table;
    for (const auto& [A, productions] : G) {
        for (const auto& prod : productions) {
            // FIRST of the production
            SSet firstp = Surab::Compiler::FIRST(prod, G, FIRST);

            // For every terminal in FIRST(prod)
            for (const auto& a : firstp) {
                if (a != EPSILON) {
                    table[A][a].push_back(prod);
                }
            }

            // If the production can derive epsilon,
            // add it to every terminal in FOLLOW(A)
            if (firstp.contains(EPSILON)) {
                SSet followA = Surab::Compiler::FOLLOW(A, G, FIRST, FOLLOW);

                for (const auto& b : followA) {
                    table[A][b].push_back(prod);
                }
            }
        }
    }
    return table;
}



} // namespace Compiler
}
