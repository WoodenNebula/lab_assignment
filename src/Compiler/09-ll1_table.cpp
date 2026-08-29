/*
Construct an LL(1) parsing table from a grammar (uses FIRST/FOLLOW computation).
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <sstream>

using GrammarClass = Surab::Compiler::Grammar::GrammarClass;

using Token_t = Surab::Compiler::Token_t;
using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;

using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;

using ParseTable_t = Surab::Compiler::Parser::ParseTable_t;

ParseTable_t ConstructParseTable(
    const GrammarClass& G,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap,
    const std::unordered_map<Token_t, OrderedTokenSet>& FollowResMap
) {
    ParseTable_t table;
    for (const auto& [A, productionList] : G) {
        for (const TokenizedProduction_t& prod : productionList) {
            // FIRST of the production
            OrderedTokenSet firstp = Surab::Compiler::Parser::FIRST(G, prod.at(0), FirstResMap);

            // For every terminal in FIRST(prod)
            for (const Token_t& a : firstp) {
                if (a != EPSILON) {
                    // If the cell is already occupied, there's a conflict

                    bool cellOccupied = table.contains(A) && table.at(A).contains(a) && !table.at(A).at(a).empty();
                    if (cellOccupied) {
                        Surab::LogError("Conflict in parse table for [{}, {}]", A, a);
                        table[A][a] += " | ";
                        // break;
                    }
                    table[A][a] += A + " -> " + GrammarClass::FormatTokens(prod);
                }
            }

            // If the production can derive epsilon,
            // add it to every terminal in FOLLOW(A)
            if (firstp.contains(EPSILON)) {
                OrderedTokenSet followA = Surab::Compiler::Parser::FOLLOW(G, A, FirstResMap, FollowResMap);

                for (const auto& b : followA) {
                    bool cellOccupied = table.contains(A) && table.at(A).contains(b) && !table.at(A).at(b).empty();
                    if (cellOccupied) {
                        Surab::LogError("Conflict in parse table for [{}, {}]", A, b);
                        break;
                    }
                    table[A][b] += A + " -> " + GrammarClass::FormatTokens(prod);
                }
            }
        }
    }
    return table;
}

int main() {
    Header("LL(1) Parsing Table Builder");

    std::stringstream grammarString(R"(
        S -> L = R
        S -> R
        L -> * R
        L -> id
        R -> L
    )");

    GrammarClass G(grammarString, "E");

    Surab::Log("\nOriginal Grammar:");
    G.PrintGrammar();

    std::unordered_map<Token_t, OrderedTokenSet> FirstResMap = Surab::Compiler::Parser::ComputeFirst(G);
    std::unordered_map<Token_t, OrderedTokenSet> FollowResMap = Surab::Compiler::Parser::ComputeFollow(G, FirstResMap);

    ParseTable_t table = ConstructParseTable(G, FirstResMap, FollowResMap);

    Surab::Log("\nLL(1) Parsing Table:");
    Surab::Compiler::Parser::PrintParseTable(table, "Non-Terminal");
    Footer();
    return 0;
}
