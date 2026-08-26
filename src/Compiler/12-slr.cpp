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

#include <map>
#include <print>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Token_t = Surab::Compiler::Token_t;
using Grammar_t = Surab::Compiler::Grammar_t;
using GrammarList_t = Surab::Compiler::GrammarList_t;
using SSet_t = Surab::Compiler::SSet_t;

using ActionTable_t = Surab::Compiler::Parser::ActionTable_t;
using GotoTable_t = Surab::Compiler::Parser::GotoTable_t;

using SLRItem = Surab::Compiler::SLR::SItem;
using SLRItemSet = Surab::Compiler::SLR::SItemSet;
using CanonicalState = Surab::Compiler::SLR::SCanonicalState;


SLRItemSet ClosureLR0(
    const SLRItemSet& State,
    const GrammarList_t& GrammarList,
    const SSet_t& NonTerminals
) {
    SLRItemSet closure = State;
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto& [prodIndex, dotIndex] : closure) {
            if (prodIndex >= GrammarList.size())
                continue;

            const auto& [lhs, rhs] = GrammarList[prodIndex];
            if (dotIndex >= rhs.size())
                continue;

            const Token_t nextSymbol = rhs[dotIndex];
            if (!Surab::Compiler::Grammar::IsNonTerminal(nextSymbol, NonTerminals))
                continue;

            // Add all productions of the non-terminal symbol to the closure
            for (size_t i = 0; i < GrammarList.size(); ++i) {
                const auto& [candidateLhs, candidateRhs] = GrammarList[i];
                if (candidateLhs != nextSymbol)
                    continue;
                const SLRItem next = { i, 0 };
                if (closure.insert(next).second)
                    changed = true;
            }
        }
    }

    return closure;
}

SLRItemSet GotoLR0(
    const SLRItemSet& State,
    const Token_t& Symbol,
    const GrammarList_t& GrammarList,
    const SSet_t& NonTerminals
) {
    SLRItemSet movedState;
    for (const auto& [prodIndex, dot] : State) {
        const auto& [lhs, rhs] = GrammarList[prodIndex];
        bool isValidIndex = dot < rhs.size();
        if (!isValidIndex)
            continue;
        bool isMatchingSymbol = rhs[dot] == Symbol;
        if (isMatchingSymbol) {
            movedState.insert({ prodIndex, dot + 1 });
        }
    }
    return ClosureLR0(movedState, GrammarList, NonTerminals);
}

std::vector<CanonicalState> CanonicalCollectionLR0(const GrammarList_t& GrammarList, const SSet_t& NonTerminals) {
    std::vector<CanonicalState> states;
    std::vector<SLRItemSet> queue;

    SLRItemSet start = ClosureLR0({ { 0, 0 } }, GrammarList, NonTerminals);
    queue.push_back(start);
    states.emplace_back(0, 0, GrammarList[0].first, start);

    std::map<std::string, size_t> seen;
    seen[Surab::Compiler::SLR::StateKey(GrammarList, start)] = 0;

    for (size_t index = 0; index < queue.size(); ++index) {
        const SLRItemSet current = queue[index];
        std::vector<Token_t> symbols;
        // collect all the unique symbols that are immediately after the dot in the current state
        for (const auto& [prodIndex, dot] : current) {
            const auto& [lhs, rhs] = GrammarList[prodIndex];
            if (dot < rhs.size() && !std::ranges::contains(symbols, rhs[dot])) {
                symbols.push_back(rhs[dot]);
            }
        }

        for (const auto& symbol : symbols) {
            SLRItemSet next = GotoLR0(current, symbol, GrammarList, NonTerminals);
            if (next.empty())
                continue;
            const std::string key = Surab::Compiler::SLR::StateKey(GrammarList, next);
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
    const std::vector<CanonicalState>& States,
    const GrammarList_t& GrammarList,
    const Token_t& StartSymbol,
    const std::map<Token_t, SSet_t>& FollowMap,
    const SSet_t& NonTerminals
) {
    ActionTable_t actionTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const SLRItemSet state = States[stateId].Items;
        for (const auto& item : state) {
            const auto& [prodIndex, dot] = item;
            const auto& [lhs, rhs] = GrammarList[prodIndex];

            if (dot == rhs.size()) {
                if (lhs == StartSymbol + "'") {
                    actionTable[stateId]["$"] = "accept";
                }
                else {
                    for (const auto& lookahead : FollowMap.at(lhs)) {
                        actionTable[stateId][lookahead] = "reduce: " + Surab::Compiler::FormatProduction({ lhs, rhs });
                    }
                }
                continue;
            }

            const Token_t symbol = rhs[dot];
            if (!Surab::Compiler::Grammar::IsNonTerminal(symbol, NonTerminals)) {
                SLRItemSet shifted = GotoLR0(state, symbol, GrammarList, NonTerminals);
                size_t target = std::numeric_limits<size_t>::max();
                for (size_t i = 0; i < States.size(); ++i) {
                    if (States[i].Items == shifted) {
                        target = i;
                        break;
                    }
                }
                if (target != std::numeric_limits<size_t>::max()) {
                    actionTable[stateId][symbol] = "shift: " + StartSymbol + std::to_string(target);
                }
            }
        }
    }
    return actionTable;
}

GotoTable_t ConstructGotoTable(
    const std::vector<CanonicalState>& States,
    const GrammarList_t& GrammarList,
    const SSet_t& NonTerminals
) {
    GotoTable_t gotoTable;
    for (size_t stateId = 0; stateId < States.size(); ++stateId) {
        const SLRItemSet state = States[stateId].Items;

        for (const auto& nt : NonTerminals) {
            SLRItemSet shifted = GotoLR0(state, nt, GrammarList, NonTerminals);
            if (shifted.empty()) continue;
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
        E -> E+T | T
        T -> T*F | F
        F -> (E) | id
    )");

    const Token_t startSymbol = "E";
    const Token_t augmentedStartSymbol = startSymbol + "'";

    Grammar_t augmentedGrammar = Surab::Compiler::AugmentGrammar(Surab::Compiler::ParseGrammarFromString(grammarString), startSymbol);
    Surab::Log("\nAugmented Grammar:");
    Surab::Compiler::PrintGrammar(augmentedGrammar);

    const auto firstMap = Surab::Compiler::Parser::ComputeFirst(augmentedGrammar);
    const auto followMap = Surab::Compiler::Parser::ComputeFollow(augmentedGrammar, firstMap, augmentedStartSymbol);

    const auto [nonTerminals, rawTerminals] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(augmentedGrammar);
    const auto grammarList = Surab::Compiler::ToProductionVector(augmentedGrammar, startSymbol);

    Surab::Log("\nGrammar List: ");
    Surab::Compiler::PrintGrammarList(grammarList);

    const auto states = CanonicalCollectionLR0(grammarList, nonTerminals);
    Surab::Compiler::SLR::PrintCanonicalStates(grammarList, states);

    Surab::Log("\nFOLLOW sets:");
    for (auto& [A, _] : followMap) {
        Surab::LogSuccess("FOLLOW({}) = {} ", A, followMap.at(A));
    }

    ActionTable_t actionTable = ConstructActionTable(states, grammarList, startSymbol, followMap, nonTerminals);
    GotoTable_t gotoTable = ConstructGotoTable(states, grammarList, nonTerminals);

    Surab::Log("\nSLR(1) Parsing Table:");
    Surab::Compiler::Parser::PrintCombinedParseTable(actionTable, gotoTable, nonTerminals, rawTerminals);

    Footer();
    return 0;
}
