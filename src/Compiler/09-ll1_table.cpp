/*
Construct an LL(1) parsing table from a grammar (uses FIRST/FOLLOW computation).
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>

using Token_t = Surab::Compiler::Token_t;
using Grammar_t = Surab::Compiler::Grammar_t;
using SSet_t = Surab::Compiler::SSet_t;

using ParseTable_t = Surab::Compiler::ParseTable_t;

void PrintTable(const ParseTable_t& table) {
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
    std::map<std::pair<Token_t, Token_t>, std::string> cells;

    for (const Token_t& A : NonTerminals) {
        for (const Token_t& a : Terminals) {

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

            Surab::Compiler::Production_t production = table.at(A).at(a);
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


ParseTable_t ConstructParseTable(
    const Grammar_t& G,
    const std::unordered_map<Token_t, SSet_t>& FirstResMap,
    const std::unordered_map<Token_t, SSet_t>& FollowResMap
) {
    ParseTable_t table;
    for (const auto& [A, productionList] : G) {
        for (const auto& prod : productionList) {
            // FIRST of the production
            SSet_t firstp = Surab::Compiler::FIRST(prod, G, FirstResMap);

            // For every terminal in FIRST(prod)
            for (const Token_t& a : firstp) {
                if (a != EPSILON) {
                    // If the cell is already occupied, there's a conflict

                    bool cellOccupied = table.contains(A) && table.at(A).contains(a) && !table.at(A).at(a).empty();
                    if (cellOccupied) {
                        Surab::LogError("Conflict in parse table for [{}, {}]", A, a);
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
                        Surab::LogError("Conflict in parse table for [{}, {}]", A, b);
                        break;
                    }
                    table[A][b] = prod;
                }
            }
        }
    }
    return table;
}

int main() {
    Header("LL(1) Parsing Table Builder");

    std::stringstream grammarString(R"(
        E -> TE'
        E' -> +TE' | ∈
        F -> id | (E)
        T' -> *FT' | ∈
        T -> FT'
    )");

    Grammar_t G = Surab::Compiler::ParseGrammarFromString(grammarString);

    Surab::Log("\nOriginal Grammar:");
    Surab::Compiler::PrintGrammar(G);

    std::unordered_map<Token_t, SSet_t> FirstResMap = Surab::Compiler::ComputeFirst(G);
    std::unordered_map<Token_t, SSet_t> FollowResMap = Surab::Compiler::ComputeFollow(G, FirstResMap, "E");

    ParseTable_t table = ConstructParseTable(G, FirstResMap, FollowResMap);

    Surab::Log("\nLL(1) Parsing Table:");
    PrintTable(table);
    Footer();
    return 0;
}
