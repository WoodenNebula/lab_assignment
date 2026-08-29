#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <stack>
#include <map>

using GrammarClass = Surab::Compiler::Grammar::GrammarClass;

using Token_t = Surab::Compiler::Token_t;
using TokenizedProduction_t = Surab::Compiler::Grammar::TokenizedProduction_t;

using OrderedTokenSet = Surab::Compiler::OrderedTokenSet;
using ParseTable_t = Surab::Compiler::Parser::ParseTable_t;

int main() {
    Header("Top-Down Non-Recursive Descent (LL(1) Parser)");
    std::vector<Token_t> inputTokens = { "id", "+", "id", "*", "id", "$" };

    std::stringstream grammarString(R"(
        E -> TE'
        E' -> +TE' | ∈
        F -> id | (E)
        T' -> *FT' | ∈
        T -> FT'
    )");

    GrammarClass G(grammarString, "E");

    Surab::Log("\n===Original Grammar===");
    G.PrintGrammar();

    // auto [nonTerminalSet, terminalSet] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);
    OrderedTokenSet nonTerminalSet = G.GetNonTerminals();

    std::unordered_map<Token_t, OrderedTokenSet> FIRST = Surab::Compiler::Parser::ComputeFirst(G);
    std::unordered_map<Token_t, OrderedTokenSet> FOLLOW = Surab::Compiler::Parser::ComputeFollow(G, FIRST);

    ParseTable_t table = Surab::Compiler::Parser::ConstructParseTable(G, FIRST, FOLLOW);
    Surab::Log("\n===Parse Table===");
    Surab::Compiler::Parser::PrintParseTable(table, "Non-Terminal");

    std::stack<Token_t> stack;
    stack.push("$");
    stack.push(G.GetStartSymbol());
    size_t ip = 0;

    Surab::Log("\n===Parsing Steps===");
    Surab::Compiler::Parser::PrintParsingSeparator();
    Surab::Compiler::Parser::PrintParsingStep("Stack", "Input", "Action");
    Surab::Compiler::Parser::PrintParsingSeparator();

    while (!stack.empty()) {
        // print stack
        std::string stackStr;
        std::stack<Token_t> tmp = stack;
        while (!tmp.empty()) {
            stackStr = tmp.top() + stackStr;
            tmp.pop();
        }

        std::string inputStr;
        for (size_t i = ip; i < inputTokens.size(); ++i) {
            inputStr += inputTokens[i] + " ";
        }
        inputStr = Surab::Trim(inputStr);

        Surab::Compiler::Token_t X = stack.top();
        Surab::Compiler::Token_t a = inputTokens[ip];

        // End of stack and input
        if (X == a && X == "$") {
            Surab::Compiler::Parser::PrintParsingStep(stackStr, inputStr, "Accept", GREEN);
            break;
        }

        // terminal match
        if (X == a) {
            stack.pop();
            ++ip;
            Surab::Compiler::Parser::PrintParsingStep(stackStr, inputStr, std::format("POP({})", X));
            continue;
        }

        // No Production for non-terminal, error
        if (X != "$" && !nonTerminalSet.contains(X)) {
            Surab::Compiler::Parser::PrintParsingSeparator();
            Surab::LogError("Error: unexpected terminal {}", X);
            Surab::Compiler::Parser::PrintParsingStep(stackStr, inputStr, "");
            break;
        }

        // No LL(1) table entry for non-terminal, error
        if (!table.contains(X) || !table.at(X).contains(a)) {
            Surab::Compiler::Parser::PrintParsingSeparator();
            Surab::LogError("Error: no table entry for {} with input {}", X, a);
            Surab::Compiler::Parser::PrintParsingStep(stackStr, inputStr, "");
            break;
        }

        TokenizedProduction_t production = GrammarClass::TokenizeProduction(GrammarClass::ParseProductionStringLine(table.at(X).at(a)).second, nonTerminalSet);
        stack.pop();
        Surab::Compiler::Parser::PrintParsingStep(stackStr, inputStr, std::format("{} -> {}", X, production[0]));

        // push rhs in reverse (ignore epsilon)
        if (!(production.size() == 1 && production[0] == EPSILON)) {
            for (auto rit = production.rbegin(); rit != production.rend(); ++rit) {
                stack.push(*rit);
            }
        }
    }
    Surab::Compiler::Parser::PrintParsingSeparator();

    Footer();
    return 0;
}