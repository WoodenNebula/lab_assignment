#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <map>

using GrammarClass = Surab::Compiler::Grammar::GrammarClass;

using Token_t = Surab::Compiler::Token_t;
using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;

using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;
using ParseTable_t = Surab::Compiler::Parser::ParseTable_t;


void PrintState(
    const std::vector<Token_t>& Stack,
    const std::vector<Token_t>& InputTokens,
    const size_t& InputIndex,
    const std::string& Action,
    const std::string& ColorCode = RESET
) {
    std::string stackStr = Surab::ToString(Stack, " ", false);
    std::vector<Token_t> inputSlice(InputTokens.begin() + InputIndex, InputTokens.end());
    std::string inputStr = Surab::ToString(inputSlice, " ", false);

    Surab::Compiler::Parser::PrintParsingStep(stackStr, inputStr, Action, ColorCode);
}

std::pair<bool, Token_t> IsHandle(const std::string& Tokens, const GrammarClass& G) {
    for (const auto& [nonTerminal, productionList] : G) {
        std::vector<std::string> productionStrings;
        for (const auto& production : productionList) {
            std::string productionStr = GrammarClass::FormatTokens(production);
            productionStrings.push_back(productionStr);
        }

        bool isHandle = std::ranges::any_of(productionStrings, [&](const std::string& prodStr) {
            return prodStr == Tokens;
            });
        if (isHandle) {
            return { true, nonTerminal };
        }
    }
    return { false, "" };
}

bool CanShift(const std::vector<Token_t>& InputTokens, const size_t& Index) {
    bool bValidIndex = Index < InputTokens.size();
    bool bNotEndMarker = bValidIndex && InputTokens.at(Index) != "$";
    return bNotEndMarker;
}

bool ShiftReduceParse(const GrammarClass& G, const std::vector<Token_t>& InputTokens, const Token_t& StartSymbol) {
    Surab::Log("\n===Parsing Steps===");
    Surab::Compiler::Parser::PrintParsingSeparator();
    Surab::Compiler::Parser::PrintParsingStep("Stack", "Input Buffer", "Action");
    Surab::Compiler::Parser::PrintParsingSeparator();

    std::vector<Token_t> stack = { "$" };
    size_t ip = 0;
    while (true) {
        bool reduced = false;
        // Try to reduce from the stack content until a handle is found or no more reductions are possible
        // using all suffix lengths
        for (size_t len = 1; len <= stack.size(); ++len) {
            std::string suffix;
            for (size_t k = stack.size() - len; k < stack.size(); ++k) {
                suffix += suffix.empty() ? stack[k] : " " + stack[k];

                auto [isHandle, nonTerminal] = IsHandle(suffix, G);
                if (isHandle) {
                    PrintState(stack, InputTokens, ip, std::format("Reduce by {} -> {}", nonTerminal, suffix));
                    for (size_t k = 0;k < len; ++k)
                        stack.pop_back();
                    stack.push_back(nonTerminal);
                    reduced = true;
                    break;
                }
            }
            if (reduced)
                break;
        }

        // if reduced, continue to next iteration without shifting
        if (reduced)
            continue;

        // shift next symbol
        if (CanShift(InputTokens, ip)) {
            PrintState(stack, InputTokens, ip, "Shift");
            stack.push_back(InputTokens.at(ip));
            ip++;
        }
        else if (stack.back() == StartSymbol && InputTokens.at(ip) == "$") {
            PrintState(stack, InputTokens, ip, "Accept", GREEN);
            Surab::Compiler::Parser::PrintParsingSeparator();
            return true;
        }
        else {
            PrintState(stack, InputTokens, ip, "Reject", RED);
            Surab::Compiler::Parser::PrintParsingSeparator();
            return false;
        }

    }
}

int main() {
    Header("Shift-Reduce Parser");

    std::stringstream grammarString(R"(
        S -> S + S | S * S | id
    )");
    GrammarClass G(grammarString, "S");

    Surab::Log("\n===Original Grammar===");
    G.PrintGrammar();

    std::vector<Token_t> inputTokens = { "id", "+", "id", "*", "id", "$" };
    Surab::Log("\nInput Tokens: {}", Surab::ToString(inputTokens));

    bool isAccepted = ShiftReduceParse(G, inputTokens, G.GetStartSymbol());

    if (isAccepted) {
        Surab::LogSuccess("\n{} is accepted by the grammar.", Surab::ToString(inputTokens));
    }
    else {
        Surab::LogError("\n{} is rejected by the grammar.", Surab::ToString(inputTokens));
    }

    Footer();
    return 0;
}
