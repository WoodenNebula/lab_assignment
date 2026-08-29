/*
Eliminate direct left recursion and perform simple left-factoring for CFGs.
Input format (one production per line):
A -> A a | b | c
Enter an empty line to finish.
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>
#include <algorithm>

using GrammarClass = Surab::Compiler::Grammar::GrammarClass;

using Token_t = Surab::Compiler::Token_t;
using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;

// Indirect left recursion is not handled in this implementation
GrammarClass EliminateDirectLeftRecursion(const GrammarClass& G) {
    GrammarClass additions;
    for (const auto& [productionSymbol, productionList] : G) {
        std::vector<TokenizedProduction_t> alpha, beta;
        for (const TokenizedProduction_t& tokenizedProduction : productionList) {
            // if p starts with A
            bool startsWithA = !tokenizedProduction.empty() && tokenizedProduction[0] == productionSymbol;
            if (startsWithA) {
                // remove prefix A
                TokenizedProduction_t rest{ tokenizedProduction.begin() + 1, tokenizedProduction.end() };
                if (rest.empty())
                    alpha.push_back({ EPSILON });
                else
                    alpha.emplace_back(rest);
            }
            else beta.emplace_back(tokenizedProduction);
        }
        if (!alpha.empty()) {
            Token_t Aprime = productionSymbol + "'";
            while (G.contains(Aprime)) {
                Aprime += "'";
            }

            std::vector<TokenizedProduction_t> newAprods;
            for (TokenizedProduction_t& b : beta) {
                b.push_back(Aprime);
                newAprods.push_back(b);
            }
            additions.insert(productionSymbol, { newAprods });

            std::vector<TokenizedProduction_t> aprimeProds;
            for (TokenizedProduction_t& a : alpha) {
                a.push_back(Aprime);
                aprimeProds.push_back(a);
            }
            aprimeProds.push_back({ EPSILON });
            additions.insert(Aprime, { aprimeProds });
        }
    }

    GrammarClass transformedGrammar = G;
    // update transformedGrammar with LeftRecursion eliminated productions
    for (const auto& [productionSymbol, productionList] : G) {
        if (additions.contains(productionSymbol)) {
            transformedGrammar[productionSymbol] = additions[productionSymbol];
            additions.erase(productionSymbol);
        }
        else {
            transformedGrammar[productionSymbol] = { productionList };
        }
    }

    // add the new A' productions
    for (const auto& [A, prods] : additions) {
        transformedGrammar[A] = prods;
    }

    return transformedGrammar;
}

GrammarClass LeftFactorGrammar(const GrammarClass& G) {
    GrammarClass transformedGrammar = G;
    for (const auto& [A, prodList] : G) {
        size_t minPrefixLength = 1;
        size_t numProdsWithSamePrefix = 0;
        size_t shortestProdLength = std::numeric_limits<size_t>::max();

        Token_t minCommonPrefixToken = prodList[0].at(0);

         // Find the number of productions that share the same prefix
        for (size_t i = 0; i < prodList.size(); i++) {
            Token_t prefixToken = prodList[i].at(0);
            if (prefixToken == minCommonPrefixToken) {
                numProdsWithSamePrefix++;
                shortestProdLength = std::min(shortestProdLength, prodList[i].size());
            }
            else { break; }
        }

        std::vector<Token_t> commonPrefixTokens;
        // Find the longest common prefix among the productions
        bool matchingPrefix = numProdsWithSamePrefix > 1;
        while (matchingPrefix && minPrefixLength <= shortestProdLength) {
            std::vector<Token_t> possiblePrefix;
            for (size_t j = 0; j < minPrefixLength + 1; j++) {
                possiblePrefix.push_back(prodList[0].at(j));
            }
            for (size_t i = 0; i < numProdsWithSamePrefix; i++) {
                if (minPrefixLength + 1 <= prodList[i].size()) {
                    std::vector<Token_t> prefix;
                    for (size_t j = 0; j < minPrefixLength + 1; j++) {
                        prefix.push_back(prodList[i].at(j));
                    }

                    if (possiblePrefix != prefix) {
                        matchingPrefix = false;
                        break;
                    }
                }
                else {
                    matchingPrefix = false;
                    break;
                }
            }
            if (matchingPrefix) {
                commonPrefixTokens = possiblePrefix;
                minPrefixLength++;
            }
        }

        if (numProdsWithSamePrefix > 1) {
            // Surab::LogSuccess("Longest common prefix for {}: '{}'", A, Surab::ToString(commonPrefixTokens, " ", true));

            Token_t Aprime = A + "'";
            while (G.contains(Aprime)) {
                Aprime += "'";
            }
            std::vector<TokenizedProduction_t> newAprods;
            for (size_t i = 0; i < numProdsWithSamePrefix; ++i) {
                TokenizedProduction_t suffix{ prodList[i].begin() + commonPrefixTokens.size(), prodList[i].end() };
                if (suffix.empty())
                    newAprods.push_back({ EPSILON });
                else
                    newAprods.emplace_back(suffix);
            }
            auto foo = transformedGrammar[Aprime];
            transformedGrammar[Aprime] = { newAprods };

            std::vector<TokenizedProduction_t> updatedProds(prodList.begin() + numProdsWithSamePrefix, prodList.end());
            commonPrefixTokens.push_back(Aprime);
            updatedProds.emplace(updatedProds.begin(), commonPrefixTokens);
            transformedGrammar[A] = updatedProds;
        }

    }
    return transformedGrammar;
}

int main() {
    Header("Grammar Transformer");

    std::stringstream grammarString(R"(
        S -> SB c | SB c B | SB c a b | b a | a | b
        B -> b c | a
    )");

    GrammarClass G(grammarString);

    Surab::Log("\nOriginal Grammar:");
    G.PrintGrammar();

    GrammarClass leftRecursionFreeGrammar = EliminateDirectLeftRecursion(G);
    Surab::LogSuccess("\nTransformed Grammar (after eliminating direct left recursion):");
    leftRecursionFreeGrammar.PrintGrammar();

    GrammarClass leftFactoredGrammar = LeftFactorGrammar(leftRecursionFreeGrammar);
    Surab::LogSuccess("\nTransformed Grammar (after left factoring):");
    leftFactoredGrammar.PrintGrammar();

    Footer();
    return 0;
}
