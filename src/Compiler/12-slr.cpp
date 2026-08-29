/*
Construct SLR(1) parsing table for the classic expression grammar:
S' -> E
E -> E + T | T
T -> T * F | F
F -> ( E ) | id

This program computes LR(0) canonical items and prints the complete SLR(1)
ACTION and GOTO tables.
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <unordered_map>
#include <print>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Token_t = Surab::Compiler::Token_t;
using GrammarClass = Surab::Compiler::Grammar::GrammarClass;
using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;

using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;
using ProductionRule = Surab::Compiler::Grammar::ProductionRule;

using ActionTable_t = Surab::Compiler::Parser::ActionTable_t;
using GotoTable_t = Surab::Compiler::Parser::GotoTable_t;

using SLRItem = Surab::Compiler::SLR::ParserItem;
using SLRItemSet = std::set<SLRItem>;
using CanonicalState = Surab::Compiler::SLR::ParserCanonicalState;

SLRItemSet ClosureLR0(
    const GrammarClass& AugmentedGrammar,
    const SLRItemSet& State
) {
    SLRItemSet closure = State;
    bool changed = true;

    const std::vector<ProductionRule>& ProductionRuleList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet& NonTerminals = AugmentedGrammar.GetNonTerminals();

    while (changed) {
        changed = false;
        for (const auto [prodIndex, dotIndex] : closure) {
            if (prodIndex >= ProductionRuleList.size())
                continue;

            const auto& [lhs, rhs] = ProductionRuleList[prodIndex];
            const auto& production = rhs[0];
            if (dotIndex >= production.size())
                continue;

            const Token_t nextSymbol = production[dotIndex];
            if (!NonTerminals.contains(nextSymbol))
                continue;

            // Add all productions of the non-terminal symbol to the closure
            for (size_t i = 0; i < ProductionRuleList.size(); ++i) {
                const auto& [candidateLhs, candidateRhs] = ProductionRuleList[i];
                if (candidateLhs != nextSymbol)
                    continue;
                const SLRItem next = { i, 0 };
                if (const auto [it, inserted] = closure.insert(next); inserted)
                    changed = true;
            }
        }
    }

    return closure;
}

SLRItemSet GotoLR0(
    const GrammarClass& AugmentedGrammar,
    const SLRItemSet& State,
    const Token_t& Symbol
) {

    const std::vector<ProductionRule>& GrammarList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet& NonTerminals = AugmentedGrammar.GetNonTerminals();

    SLRItemSet movedState;
    for (const auto& [prodIndex, dotIndex] : State) {
        const TokenizedProduction_t& tokenizedProduction = GrammarList[prodIndex].TokenizedProductionList[0];

        bool isValidIndex = dotIndex < tokenizedProduction.size();
        if (!isValidIndex)
            continue;
        bool isMatchingSymbol = tokenizedProduction[dotIndex] == Symbol;
        if (isMatchingSymbol) {
            movedState.insert({ prodIndex, dotIndex + 1 });
        }
    }
    return ClosureLR0(AugmentedGrammar, movedState);
}

std::vector<CanonicalState> CanonicalCollectionLR0(const GrammarClass& AugmentedGrammar) {
    const std::vector<ProductionRule>& ProductionRuleList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet& NonTerminals = AugmentedGrammar.GetNonTerminals();

    std::vector<CanonicalState> states;
    std::vector<SLRItemSet> queue;

    SLRItemSet start = ClosureLR0(AugmentedGrammar, { { 0, 0 } });
    queue.push_back(start);
    states.emplace_back(0, 0, ProductionRuleList[0].ProductionSymbol, start);

    std::unordered_map<std::string, size_t> seen;
    seen[Surab::Compiler::SLR::StateKey(AugmentedGrammar, start)] = 0;

    for (size_t index = 0; index < queue.size(); ++index) {
        const SLRItemSet current = queue[index];
        OrderedTokenSet symbols;
        // collect all the unique symbols that are immediately after the dot in the current state
        for (const auto& [prodIndex, dotIndex] : current) {
            const TokenizedProduction_t& production = ProductionRuleList[prodIndex].TokenizedProductionList[0];
            if (dotIndex < production.size() && production[dotIndex] != EPSILON && !symbols.contains(production[dotIndex])) {
                symbols.push_back(production[dotIndex]);
            }
        }
        for (const auto& symbol : symbols) {
            SLRItemSet next = GotoLR0(AugmentedGrammar, current, symbol);
            if (next.empty())
                continue;
            const std::string key = Surab::Compiler::SLR::StateKey(AugmentedGrammar, next);
            if (!seen.contains(key)) {
                seen[key] = states.size();
                states.emplace_back(states.size(), index, symbol, next);
                queue.push_back(next);
            }
        }
    }
    return states;
}

ActionTable_t ConstructActionTable(
    const GrammarClass& AugmentedGrammar,
    const std::vector<CanonicalState>& States,
    const std::unordered_map<Token_t, OrderedTokenSet>& FollowMap
) {
    const auto& ProductionRuleList = AugmentedGrammar.GetSplitProductions();
    const auto& AugmentedStartSymbol = AugmentedGrammar.GetStartSymbol();
    const auto& NonTerminals = AugmentedGrammar.GetNonTerminals();

    ActionTable_t actionTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const SLRItemSet state = States[stateId].Items;
        for (const auto& [prodIndex, dotIndex] : state) {
            const ProductionRule rule = ProductionRuleList[prodIndex];
            const TokenizedProduction_t& production = rule.TokenizedProductionList[0];

            if (dotIndex == production.size()) {
                if (rule.ProductionSymbol == AugmentedStartSymbol) {
                    actionTable[stateId]["$"] = "ACCEPT";
                }
                else {
                    for (const auto& nextChar : FollowMap.at(rule.ProductionSymbol)) {
                        actionTable[stateId][nextChar] = std::format("reduce [{}]: {}", prodIndex, rule.ToString());
                    }
                }
                continue;
            }

            const Token_t symbolAfterDot = production[dotIndex];
            if (!NonTerminals.contains(symbolAfterDot)) {
                const auto& AugmentedStartSymbol = AugmentedGrammar.GetStartSymbol();
                const auto& NonTerminals = AugmentedGrammar.GetNonTerminals();
                SLRItemSet shifted = GotoLR0(AugmentedGrammar, state, symbolAfterDot);
                size_t target = std::numeric_limits<size_t>::max(); // invalid state
                for (size_t i = 0; i < States.size(); ++i) {
                    if (States[i].Items == shifted) {
                        target = i;
                        break;
                    }
                }
                if (target != std::numeric_limits<size_t>::max()) {
                    actionTable[stateId][symbolAfterDot] = "shift: " + std::to_string(target);
                }
            }
        }
    }
    return actionTable;
}

GotoTable_t ConstructGotoTable(
    const GrammarClass& AugmentedGrammar,
    const std::vector<CanonicalState>& States
) {
    const auto& GrammarList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet& NonTerminals = AugmentedGrammar.GetNonTerminals();

    GotoTable_t gotoTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const SLRItemSet state = States[stateId].Items;

        for (const auto& nt : NonTerminals) {
            SLRItemSet shifted = GotoLR0(AugmentedGrammar, state, nt);
            if (shifted.empty())
                continue;
            for (size_t i = 0; i < States.size(); ++i) {
                if (States[i].Items == shifted) {
                    gotoTable[stateId][nt] = i;
                    break;
                }
            }
        }
    }
    return gotoTable;
}

int main() {
    Header("SLR(1) Parsing Table");

    std::stringstream grammarString(R"(
        C -> AB
        A -> a
        B -> a
    )");

    GrammarClass originalGrammar(grammarString, "C");
    GrammarClass augmentedGrammar = originalGrammar.GetAugmentedGrammar();

    Surab::Log("\nAugmented Grammar:");
    augmentedGrammar.PrintGrammar();

    Surab::Log("\nProduction List: ");
    augmentedGrammar.PrintProductionList();

    const auto firstMap = Surab::Compiler::Parser::ComputeFirst(augmentedGrammar);
    const auto followMap = Surab::Compiler::Parser::ComputeFollow(augmentedGrammar, firstMap);

    const auto states = CanonicalCollectionLR0(augmentedGrammar);
    Surab::Compiler::SLR::PrintCanonicalStates(augmentedGrammar, states);

    Surab::Log("\nFOLLOW sets:");
    for (auto& [A, _] : followMap) {
        Surab::LogSuccess("FOLLOW({}) = {} ", A, followMap.at(A));
    }

    ActionTable_t actionTable = ConstructActionTable(augmentedGrammar, states, followMap);
    GotoTable_t gotoTable = ConstructGotoTable(augmentedGrammar, states);

    Surab::Log("\nSLR(1) Parsing Table:");
    Surab::Compiler::Parser::PrintCombinedParseTable(augmentedGrammar, actionTable, gotoTable);

    Footer();
    return 0;
}
