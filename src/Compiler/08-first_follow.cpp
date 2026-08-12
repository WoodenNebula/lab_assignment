/*
Compute FIRST and FOLLOW sets for a non-recursive CFG.
Input productions one per line: A -> alpha | beta
Empty line to finish. Nonterminals assumed uppercase start.
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <sstream>

using ProdList = Surab::Compiler::ProdList;
using Grammar = Surab::Compiler::Grammar;

using SSet = Surab::Compiler::SSet;

SSet FIRST(std::string symbol, const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST_LIST) {
    if (FIRST_LIST.contains(symbol)) {
        return FIRST_LIST.at(symbol);
    }

    auto [NT, T] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

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
    auto symbols = Surab::Compiler::TokenizeProduction(symbol, NT);

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
    auto [NT, _] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

    std::unordered_map<std::string, SSet> FIRST_LIST;

    for (const auto& X : NT)
        FIRST_LIST[X] = FIRST(X, G, FIRST_LIST);

    return FIRST_LIST;
}

SSet FOLLOW(std::string symbol, const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST, std::unordered_map<std::string, SSet>& FOLLOW_LIST) {
    auto [NT, T] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);
    SSet followOfSymbol;

    for (const auto& [A, prods] : G) {
        for (const auto& prod : prods) {
            auto symbols = Surab::Compiler::TokenizeProduction(prod, NT);

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

std::unordered_map<std::string, SSet> ComputeFollow(const Grammar& G, const std::unordered_map<std::string, SSet>& FIRST) {
    auto [NT, _] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

    std::unordered_map<std::string, SSet> FOLLOW_LIST;
    // Initialize FOLLOW of start symbol
    FOLLOW_LIST["S"].insert("$");

    for (const auto& X : NT)
        FOLLOW_LIST[X] = FOLLOW(X, G, FIRST, FOLLOW_LIST);

    return FOLLOW_LIST;
}



int main() {
    Header("FIRST and FOLLOW Calculator");

    std::stringstream grammarString(R"(
        F -> id | (E)
        E -> TG
        G -> +TG | ∈
        T -> FH
        H -> *FH | ∈
    )");


    Grammar G = Surab::Compiler::ParseGrammarFromString(grammarString);

    Surab::Log("\nOriginal Grammar:");
    Surab::Compiler::PrintGrammar(G);

    std::unordered_map<std::string, SSet> FIRST = ComputeFirst(G);
    std::unordered_map<std::string, SSet> FOLLOW = ComputeFollow(G, FIRST);

    Surab::Log("\nFIRST sets:");
    for (auto& [A, _] : G) {
        Surab::LogSuccess("FIRST({}) = {} ", A, FIRST[A]);
    }

    Surab::Log("\nFOLLOW sets:");
    for (auto& [A, _] : G) {
        Surab::LogSuccess("FOLLOW({}) = {} ", A, FOLLOW[A]);
    }

    Footer();
    return 0;
}
