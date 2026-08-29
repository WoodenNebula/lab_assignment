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

using Grammar_t = Surab::Compiler::Grammar::GrammarClass;

using Token_t = Surab::Compiler::Token_t;
using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;

using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;

OrderedTokenSet FIRST(
    const Grammar_t& G,
    Token_t Token,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap
) {
    if (FirstResMap.contains(Token)) {
        return FirstResMap.at(Token);
    }

    const OrderedTokenSet NT = G.GetNonTerminals();

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
    // Symbol is a production or terminal
    std::vector<Token_t> tokenList = Surab::Compiler::Grammar::GrammarClass::TokenizeProduction(Token, NT);

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


std::unordered_map<Token_t, OrderedTokenSet> ComputeFirst(const Grammar_t& G) {
    const OrderedTokenSet NT = G.GetNonTerminals();

    std::unordered_map<Token_t, OrderedTokenSet> FIRST_LIST;

    for (const auto& X : NT)
        FIRST_LIST[X] = FIRST(G, X, FIRST_LIST);

    return FIRST_LIST;
}

OrderedTokenSet FOLLOW(
    const Grammar_t& G,
    const Token_t& Token,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstResMap,
    const std::unordered_map<Token_t, OrderedTokenSet>& FollowResMap,
    const Token_t& InitiatingToken = ""
) {
    const OrderedTokenSet NT = G.GetNonTerminals();
    OrderedTokenSet followOfSymbol;

    if (FollowResMap.contains(Token)) {
        auto followRes = FollowResMap.at(Token);
        followOfSymbol.insert(followRes.cbegin(), followRes.cend());
    }

    for (const auto& [A, prodList] : G) {
        for (const TokenizedProduction_t& tokenList : prodList) {
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
                    A != InitiatingToken // Ensure the stack doesnt blow up
                    ) {
                    OrderedTokenSet followOfA = FOLLOW(G, A, FirstResMap, FollowResMap, Token);
                    followOfSymbol.insert(followOfA.begin(), followOfA.end());
                }
            }
        }
    }

    return followOfSymbol;
}

std::unordered_map<Token_t, OrderedTokenSet> ComputeFollow(
    const Grammar_t& G,
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



int main() {
    Header("FIRST and FOLLOW Calculator");

    std::stringstream grammarString(R"(
        C -> AB
        A -> a
        B -> a
    )");


    Grammar_t G(grammarString, "C");

    Surab::Log("\nOriginal Grammar:");
    G.PrintGrammar();

    std::unordered_map<Token_t, OrderedTokenSet> firstResMap = ComputeFirst(G);
    std::unordered_map<Token_t, OrderedTokenSet> followResMap = ComputeFollow(G, firstResMap);

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
