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
#include <unordered_map>
#include <sstream>
#include <set>
#include <algorithm>


using Token_t = Surab::Compiler::Token_t;
using Production_t = Surab::Compiler::Production_t;
using Grammar_t = Surab::Compiler::Grammar_t;

// Indirect left recursion is not handled in this implementation
Grammar_t EliminateDirectLeftRecursion(const Grammar_t& G) {
    Grammar_t additions;
    for (const auto& [A, prods] : G) {
        std::vector<Production_t> alpha, beta;
        for (auto& p : prods) {
            // if p starts with A
            if (p.rfind(A, 0) == 0) {
                // remove prefix A
                Token_t rest = Surab::Compiler::Trim(p.substr(A.size()));
                alpha.push_back(rest.empty() ? EPSILON : rest);
            }
            else beta.push_back(Surab::Compiler::Trim(p));
        }
        if (!alpha.empty()) {
            Token_t Aprime = A + "'";
            while (G.contains(Aprime)) {
                Aprime += "'";
            }

            std::vector<Production_t> newAprods;
            for (auto& b : beta) {
                Token_t nb = b + Aprime;
                newAprods.push_back(nb);
            }
            additions.emplace(A, newAprods);

            std::vector<Production_t> aprimeProds;
            for (auto& a : alpha) {
                Token_t na = (a + Aprime);
                aprimeProds.push_back(na);
            }
            aprimeProds.push_back(EPSILON);
            additions.emplace(Aprime, aprimeProds);
        }
    }

    Grammar_t transformedGrammar = G;
    // update transformedGrammar with LeftRecursion eliminated productions
    for (const auto& [A, prods] : G) {
        if (additions.contains(A)) {
            transformedGrammar[A] = additions[A];
            additions.erase(A);
        }
        else {
            transformedGrammar[A] = prods;
        }
    }

    // add the new A' productions
    for (const auto& [A, prods] : additions) {
        transformedGrammar[A] = prods;
    }

    return transformedGrammar;
}

Grammar_t LeftFactorGrammar(const Grammar_t& G) {
    Grammar_t transformedGrammar = G;
    for (const auto& [A, prods] : G) {
        size_t minPrefixLength = 1;
        size_t numProdsWithSamePrefix = 0;
        size_t shortestProdLength = std::numeric_limits<size_t>::max();

        std::string commonPrefix = prods[0].substr(0, minPrefixLength);

        // Find the number of productions that share the same prefix
        for (size_t i = 0; i < prods.size(); i++) {
            if (prods[i].starts_with(commonPrefix)) {
                numProdsWithSamePrefix++;
                shortestProdLength = std::min(shortestProdLength, prods[i].size());
            }
            else { break; }
        }

        // Find the longest common prefix among the productions
        bool matchingPrefix = numProdsWithSamePrefix > 1;
        while (matchingPrefix && minPrefixLength <= shortestProdLength) {
            std::string possiblePrefix = prods[0].substr(0, minPrefixLength + 1);
            for (size_t i = 0; i < numProdsWithSamePrefix; i++) {
                if (minPrefixLength + 1 <= prods[i].size()) {
                    std::string prefix = prods[i].substr(0, minPrefixLength + 1);

                    if (possiblePrefix != prefix) {
                        matchingPrefix = false; break;
                    }
                }
                else { break; matchingPrefix = false; }
            }
            if (matchingPrefix) { commonPrefix = possiblePrefix; minPrefixLength++; }
        }

        if (numProdsWithSamePrefix > 1) {
            // Surab::LogSuccess("Longest common prefix for {}: '{}'", A, commonPrefix);

            Token_t Aprime = A + "'";
            while (G.contains(Aprime)) {
                Aprime += "'";
            }
            std::vector<Production_t> newAprods;
            for (size_t i = 0; i < numProdsWithSamePrefix; ++i) {
                Token_t suffix = prods[i].substr(commonPrefix.size());
                newAprods.push_back(suffix.empty() ? EPSILON : suffix);
            }
            transformedGrammar[Aprime] = newAprods;

            std::vector<Production_t> updatedProds(prods.begin() + numProdsWithSamePrefix, prods.end());
            updatedProds.emplace(updatedProds.begin(), commonPrefix + Aprime);
            transformedGrammar[A] = updatedProds;
        }

    }
    return transformedGrammar;
}

int main() {
    Header("Grammar Transformer");

    std::stringstream grammarString(R"(
        S -> SBc | SBcB | SBcab | ba | a | b
        B -> bc | a
    )");

    Grammar_t G = Surab::Compiler::ParseGrammarFromString(grammarString);

    Surab::Log("\nOriginal Grammar:");
    Surab::Compiler::PrintGrammar(G);

    Grammar_t leftFactoredGrammar = LeftFactorGrammar(G);
    Surab::LogSuccess("\nTransformed Grammar (after left factoring):");
    Surab::Compiler::PrintGrammar(leftFactoredGrammar);

    Grammar_t leftRecursionFreeGrammar = EliminateDirectLeftRecursion(leftFactoredGrammar);
    Surab::LogSuccess("\nTransformed Grammar (after eliminating direct left recursion):");
    Surab::Compiler::PrintGrammar(leftRecursionFreeGrammar);


    Footer();
    return 0;
}
