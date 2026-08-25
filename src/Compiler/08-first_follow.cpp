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
#include <map>
#include <set>
#include <sstream>

using Grammar_t = Surab::Compiler::Grammar_t;
using Token_t = Surab::Compiler::Token_t;
using SSet_t = Surab::Compiler::SSet_t;

SSet_t FIRST(
    Token_t Token,
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap
) {
    if (FirstResMap.contains(Token)) {
        return FirstResMap.at(Token);
    }

    auto [NT, T] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

    if (Token == EPSILON) {
        return { EPSILON };
    }

    if (NT.contains(Token)) {
        // X -> A | B | aplha | ∈
        // return FIRST(A) U FIRST(B) U FIRST(alpha) U {∈}
        SSet_t firstOfSymbol;

        for (const auto& prod : G.at(Token)) {
            SSet_t firstOfProd = FIRST(prod, G, FirstResMap);
            firstOfSymbol.insert(firstOfProd.begin(), firstOfProd.end());
        }
        return firstOfSymbol;

    }
    // Symbol is a production or terminal
    std::vector<Token_t> tokenList = Surab::Compiler::TokenizeProduction(Token, NT);

    // If it is a single terminal, return it.
    if (tokenList.size() == 1 && !NT.contains(tokenList[0]))
        return { tokenList[0] };

    // FIRST of a production.
    SSet_t firstOfProduction;
    bool nullable = true;

    for (const Token_t& token : tokenList) {
        SSet_t firstOfSym = FIRST(token, G, FirstResMap);

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
    auto [NT, _] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

    std::map<Token_t, SSet_t> FIRST_LIST;

    for (const auto& X : NT)
        FIRST_LIST[X] = FIRST(X, G, FIRST_LIST);

    return FIRST_LIST;
}

SSet_t FOLLOW(
    const Token_t& Token,
    const Grammar_t& G,
    const std::map<Token_t, SSet_t>& FirstResMap,
    const std::map<Token_t, SSet_t>& FollowResMap,
    const Token_t& InitiatingToken = ""
) {
    auto [NT, T] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);
    SSet_t followOfSymbol;

    if (FollowResMap.contains(Token)) {
        followOfSymbol.insert_range(FollowResMap.at(Token));
    }

    for (const auto& [A, prodList] : G) {
        for (const auto& production : prodList) {
            std::vector<Token_t> tokenList = Surab::Compiler::TokenizeProduction(production, NT);

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
                    const SSet_t& firstBeta = FirstResMap.at(beta);

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
                    A != InitiatingToken // Ensure the stack doesnt blow up!
                    ) {
                    SSet_t followOfA = FOLLOW(A, G, FirstResMap, FollowResMap, Token);
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
    auto [NT, _] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

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



int main() {
    Header("FIRST and FOLLOW Calculator");

    std::stringstream grammarString(R"(
        S -> AS | b
        A -> bAA' | aA'
        A' -> SAA' | ∈
    )");


    Grammar_t G = Surab::Compiler::ParseGrammarFromString(grammarString);

    Surab::Log("\nOriginal Grammar:");
    Surab::Compiler::PrintGrammar(G);

    std::map<Token_t, SSet_t> firstResMap = ComputeFirst(G);
    std::map<Token_t, SSet_t> followResMap = ComputeFollow(G, firstResMap, "S");

    Surab::Log("\nFIRST sets:");
    for (auto& [A, _] : G) {
        Surab::LogSuccess("FIRST({}) = {} ", A, firstResMap[A]);
    }

    Surab::Log("\nFOLLOW sets:");
    for (auto& [A, _] : G) {
        Surab::LogSuccess("FOLLOW({}) = {} ", A, followResMap[A]);
    }

    Footer();
    return 0;
}
