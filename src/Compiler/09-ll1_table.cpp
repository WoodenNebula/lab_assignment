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

using Grammar = Surab::Compiler::Grammar;
using ProdList = Surab::Compiler::ProdList;
using SSet = Surab::Compiler::SSet;

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

int main() {
    Header("LL(1) Parsing Table Builder");

    std::stringstream grammarString(R"(
        E -> TE'
        E' -> +TE' | ∈
        F -> id | (E)
        T' -> *FT' | ∈
        T -> FT'
    )");

    Grammar G = Surab::Compiler::ParseGrammarFromString(grammarString);

    Surab::Log("\nOriginal Grammar:");
    Surab::Compiler::PrintGrammar(G);

    std::unordered_map<std::string, SSet> FIRST, FOLLOW;

    FIRST = Surab::Compiler::ComputeFirst(G);
    FOLLOW = Surab::Compiler::ComputeFollow(G, FIRST, "E");

    ParseTable table = ConstructParseTable(G, FIRST, FOLLOW);

    Surab::Log("\nLL(1) Parsing Table:");
    PrintTable(table);
    Footer();
    return 0;
}
