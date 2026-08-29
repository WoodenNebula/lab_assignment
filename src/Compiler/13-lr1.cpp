/*
Generate canonical LR(1) items and construct the LR(1) parsing table.
This version prints all canonical LR(1) item states and the ACTION/GOTO tables.
*/

#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <map>
#include <print>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using Token_t = Surab::Compiler::Token_t;
using GrammarClass = Surab::Compiler::Grammar::GrammarClass;
using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;

using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;
using ProductionRule = Surab::Compiler::Grammar::ProductionRule;

using ActionTable_t = Surab::Compiler::Parser::ActionTable_t;
using GotoTable_t = Surab::Compiler::Parser::GotoTable_t;

using LR1Item = Surab::Compiler::LR1::ParserItem;
using LR1ItemSet = std::set<LR1Item>;
using LR1CanonicalState = Surab::Compiler::LR1::ParserCanonicalState;

OrderedTokenSet FirstOfSequence(
    const std::vector<Token_t>& Sequence,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap,
    const OrderedTokenSet& NonTerminals
) {
    OrderedTokenSet result;
    bool nullable = true;

    for (const auto& symbol : Sequence) {
        if (symbol == EPSILON) {
            result.insert(EPSILON);
            continue;
        }

        if (NonTerminals.contains(symbol)) {
            for (const auto& x : FirstMap.at(symbol)) {
                if (x != EPSILON) result.insert(x);
            }
            if (!FirstMap.at(symbol).contains(EPSILON)) {
                nullable = false;
                break;
            }
        }
        else {
            result.insert(symbol);
            nullable = false;
            break;
        }
    }

    if (nullable)
        result.insert(EPSILON);
    return result;
}


LR1ItemSet ClosureLR1(
    const GrammarClass& AugmentedGrammar,
    const LR1ItemSet& State,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const auto GrammarList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet NonTerminals = AugmentedGrammar.GetNonTerminals();

    LR1ItemSet closure = State;
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto& [prodIndex, dotIndex, lookahead] : closure) {
            if (prodIndex >= GrammarList.size())
                continue;

            // const auto& [lhs, rhs] = GrammarList[prodIndex];
            const TokenizedProduction_t production = GrammarList[prodIndex].TokenizedProductionList[0];
            if (dotIndex >= production.size())
                continue;

            const Token_t nextSymbol = production[dotIndex];
            if (!NonTerminals.contains(nextSymbol))
                continue;

            std::vector<Token_t> beta(production.begin() + dotIndex + 1, production.end());
            beta.push_back(lookahead);
            const OrderedTokenSet firstBeta = FirstOfSequence(beta, FirstMap, NonTerminals);

            for (size_t i = 0; i < GrammarList.size(); ++i) {
                const auto& [candidateSymbol, _] = GrammarList[i];
                if (candidateSymbol != nextSymbol)
                    continue;

                for (const auto& a : firstBeta) {
                    if (a == EPSILON)
                        continue;
                    const LR1Item next = { i, 0, a };
                    if (auto [_, wasInserted] = closure.insert(next); wasInserted)
                        changed = true;
                }
            }
        }
    }

    return closure;
}

LR1ItemSet GotoLR1(
    const GrammarClass& AugmentedGrammar,
    const LR1ItemSet& State,
    const Token_t& Symbol,
    const std::unordered_map<Token_t, OrderedTokenSet>& Firstmap
) {
    const auto GrammarList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet NonTerminals = AugmentedGrammar.GetNonTerminals();

    LR1ItemSet movedState;
    for (const auto& [prodIndex, dotIndex, lookAhead] : State) {
        const TokenizedProduction_t& production = GrammarList[prodIndex].TokenizedProductionList[0];
        bool isValidIndex = dotIndex < production.size();
        if (!isValidIndex)
            continue;
        bool isMatchingSymbol = production[dotIndex] == Symbol;
        if (isMatchingSymbol) {
            movedState.insert({ prodIndex, dotIndex + 1, lookAhead });
        }
    }
    return ClosureLR1(AugmentedGrammar, movedState, Firstmap);
}

std::vector<LR1CanonicalState> CanonicalCollectionLR1(
    const GrammarClass& AugmentedGrammar,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {

    const auto& GrammarList = AugmentedGrammar.GetSplitProductions();
    const OrderedTokenSet& NonTerminals = AugmentedGrammar.GetNonTerminals();

    std::vector<LR1CanonicalState> states;
    std::vector<LR1ItemSet> queue;

    LR1ItemSet start = ClosureLR1(AugmentedGrammar, { { 0, 0, "$" } }, FirstMap);

    queue.push_back(start);
    states.emplace_back(0, 0, "$", start);

    std::unordered_map<std::string, size_t> seen;
    seen[Surab::Compiler::LR1::StateKey(AugmentedGrammar, start)] = 0;

    for (size_t index = 0; index < queue.size(); ++index) {
        const LR1ItemSet current = queue[index];
        Surab::Log("{}Current State I[{}] = [{}]{}", CYAN, index, Surab::Compiler::LR1::StateKey(AugmentedGrammar, current), RESET);
        OrderedTokenSet symbols;
        // collect all the unique symbols that are immediately after the dot in the current state
        for (const auto& [prodIndex, dotIndex, lookahead] : current) {
            const TokenizedProduction_t& production = GrammarList[prodIndex].TokenizedProductionList[0];
            if (dotIndex < production.size()) {
                if (dotIndex < production.size() && production[dotIndex] != EPSILON && !symbols.contains(production[dotIndex])) {
                    symbols.push_back(production[dotIndex]);
                }
            }
        }
        for (const auto& symbol : symbols) {
            const LR1ItemSet next = GotoLR1(AugmentedGrammar, current, symbol, FirstMap);
            if (next.empty())
                continue;
            const std::string key = Surab::Compiler::LR1::StateKey(AugmentedGrammar, next);
            if (!seen.contains(key)) {
                seen[key] = states.size();
                states.emplace_back(states.size(), index, symbol, next);
                Surab::Log("{}I[{}] = goto(I[{}], {}) = [{}]{}", CYAN, states.size() - 1, index, symbol, key, RESET);
                queue.push_back(next);
            }
        }
    }
    return states;
}



ActionTable_t ConstructActionTable(
    const GrammarClass& AugmentedGrammar,
    const std::vector<LR1CanonicalState>& States,
    const std::unordered_map<Token_t, OrderedTokenSet>& FollowMap
) {
    const auto& GrammarList = AugmentedGrammar.GetSplitProductions();
    const Token_t& StartSymbol = AugmentedGrammar.GetStartSymbol();
    const OrderedTokenSet& NonTerminals = AugmentedGrammar.GetNonTerminals();

    ActionTable_t actionTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const LR1ItemSet state = States[stateId].Items;
        for (const auto& [prodIndex, dotIndex, lookAhead] : state) {
            const ProductionRule rule = GrammarList[prodIndex];
            const TokenizedProduction_t& production = rule.TokenizedProductionList[0];

            if (dotIndex == production.size()) {
                if (rule.ProductionSymbol == StartSymbol) {
                    actionTable[stateId]["$"] = "ACCEPT";
                }
                else {
                    actionTable[stateId][lookAhead] = std::format("reduce [{}]: {}", prodIndex, rule.ToString());
                }
            }
            else {
                const Token_t symbolAfterDot = production[dotIndex];
                if (!NonTerminals.contains(symbolAfterDot)) {
                    const LR1ItemSet shifted = GotoLR1(AugmentedGrammar, state, symbolAfterDot, FollowMap);
                    size_t target = std::numeric_limits<size_t>::max(); // invalid state
                    for (int i = 0; i < States.size(); ++i) {
                        if (States[i].Items == shifted) {
                            target = i;
                            break;
                        }
                    }
                    if (target != std::numeric_limits<size_t>::max()) {
                        actionTable[stateId][symbolAfterDot] = "shift " + std::to_string(target);
                    }
                }
            }
        }
    }
    return actionTable;
}

GotoTable_t ConstructGotoTable(
    const GrammarClass& AugmentedGrammar,
    const std::vector<LR1CanonicalState>& States,
    const std::unordered_map<Token_t, OrderedTokenSet>& FirstMap
) {
    const OrderedTokenSet NonTerminals = AugmentedGrammar.GetNonTerminals();
    GotoTable_t gotoTable;

    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const LR1ItemSet state = States[stateId].Items;
        for (const auto& nt : NonTerminals) {
            const LR1ItemSet shifted = GotoLR1(AugmentedGrammar, state, nt, FirstMap);
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
    Header("Canonical LR(1) Parsing Table");

    std::stringstream grammarString(R"(
        S -> L=R | R
        L -> *R | id
        R -> L
    )");


    GrammarClass originalGrammar(grammarString, "S");
    GrammarClass augmentedGrammar = originalGrammar.GetAugmentedGrammar();

    Surab::Log("\nAugmented Grammar:");
    augmentedGrammar.PrintGrammar();

    Surab::Log("\nProduction List: ");
    augmentedGrammar.PrintProductionList();

    const auto firstMap = Surab::Compiler::Parser::ComputeFirst(augmentedGrammar);
    const auto followMap = Surab::Compiler::Parser::ComputeFollow(augmentedGrammar, firstMap);

    const auto states = CanonicalCollectionLR1(augmentedGrammar, firstMap);
    Surab::Compiler::LR1::PrintCanonicalStates(augmentedGrammar, states);

    ActionTable_t actionTable = ConstructActionTable(augmentedGrammar, states, followMap);
    GotoTable_t gotoTable = ConstructGotoTable(augmentedGrammar, states, firstMap);

    Surab::Compiler::Parser::PrintCombinedParseTable(augmentedGrammar, actionTable, gotoTable);

    Footer();
    return 0;
}
