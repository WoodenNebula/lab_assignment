/*
Eliminate direct left recursion and perform simple left-factoring for CFGs.
Input format (one production per line):
A -> A a | b | c
Enter an empty line to finish.
*/

#include "src/commons.hpp"

#include <print>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <set>
#include <algorithm>


std::string Trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    return s;
}

using ProdList = std::vector<std::string>;
ProdList SplitProductions(const std::string& rhs) {
    ProdList parts;
    std::stringstream ss(rhs);
    std::string item;
    while (std::getline(ss, item, '|')) {
        parts.push_back(Trim(item));
    }
    return parts;
}

using Grammar = std::unordered_map<std::string, ProdList>;
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

// Indirect left recursion is not handled in this implementation
Grammar EliminateDirectLeftRecursion(const Grammar& G) {
    Grammar additions;
    for (const auto& [A, prods] : G) {
        ProdList alpha, beta;
        for (auto& p : prods) {
            // if p starts with A
            if (p.rfind(A, 0) == 0) {
                // remove prefix A
                std::string rest = Trim(p.substr(A.size()));
                alpha.push_back(rest.empty() ? "" : rest);
            }
            else beta.push_back(Trim(p));
        }
        if (!alpha.empty()) {
            std::string Aprime = A + "'";
            while (G.contains(Aprime)) {
                Aprime += "'";
            }

            ProdList newAprods;
            for (auto& b : beta) {
                std::string nb = b + Aprime;
                newAprods.push_back(nb);
            }
            additions.emplace(A, newAprods);

            ProdList aprimeProds;
            for (auto& a : alpha) {
                std::string na = (a + Aprime);
                aprimeProds.push_back(na);
            }
            aprimeProds.push_back("∈");
            additions.emplace(Aprime, aprimeProds);
        }
    }

    Grammar transformedGrammar = G;
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

Grammar LeftFactorGrammar(const Grammar& G) {
    Grammar transformedGrammar = G;
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

        // Surab::Log("Number of productions with the same prefix for {}: {}", A, numProdsWithSamePrefix);

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

            std::string Aprime = A + "'";
            while (G.contains(Aprime)) {
                Aprime += "'";
            }
            ProdList newAprods;
            for (size_t i = 0; i < numProdsWithSamePrefix; ++i) {
                std::string suffix = prods[i].substr(commonPrefix.size());
                newAprods.push_back(suffix.empty() ? "∈" : suffix);
            }
            transformedGrammar[Aprime] = newAprods;

            ProdList updatedProds(prods.begin() + numProdsWithSamePrefix, prods.end());
            updatedProds.emplace(updatedProds.begin(), commonPrefix + Aprime);
            transformedGrammar[A] = updatedProds;
        }

    }
    return transformedGrammar;
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

int main() {
    Header("Grammar Transformer");

    std::stringstream grammarString(R"(
        S -> SBc | SBcB | SBcab | ba | a | b
        B -> bc | a
    )");

    Grammar G = ParseGrammarFromString(grammarString);

    Surab::Log("\nOriginal Grammar:");
    PrintGrammar(G);

    Grammar leftFactoredGrammar = LeftFactorGrammar(G);
    Surab::LogSuccess("\nTransformed Grammar (after left factoring):");
    PrintGrammar(leftFactoredGrammar);

    Grammar leftRecursionFreeGrammar = EliminateDirectLeftRecursion(leftFactoredGrammar);
    Surab::LogSuccess("\nTransformed Grammar (after eliminating direct left recursion):");
    PrintGrammar(leftRecursionFreeGrammar);


    Footer();
    return 0;
}
