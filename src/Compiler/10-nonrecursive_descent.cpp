#include "src/commons.hpp"
#include "src/Compiler/utils.hpp"

#include <print>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

using SSet_t = Surab::Compiler::SSet_t;
using Token_t = Surab::Compiler::Token_t;

constexpr int COL_WIDTH = 16;
constexpr int ACTION_WIDTH = 24;

void PrintTabularSteps(const std::string& stack, const std::string& input, const std::string& action, const std::string& ColorCode = RESET) {
    Surab::Log("| {:^{}} | {:^{}} | {}{:^{}}{} |", stack, COL_WIDTH, input, COL_WIDTH, ColorCode, action, ACTION_WIDTH, RESET);
}
void PrintSeparator() {
    Surab::Log("|{:-^{}}|{:-^{}}|{:-^{}}|", "", COL_WIDTH + 2, "", COL_WIDTH + 2, "", ACTION_WIDTH + 2);
}

int main() {
    Header("Top-Down Non-Recursive Descent (LL(1) Parser)");
    std::vector<std::string> inputTokens = { "id", "+", "id", "*", "id", "$" };

    std::stringstream grammarString(R"(
        E -> TE'
        E' -> +TE' | ∈
        F -> id | (E)
        T' -> *FT' | ∈
        T -> FT'
    )");
    Surab::Compiler::Token_t startSymbol = "E";

    Surab::Compiler::Grammar_t G = Surab::Compiler::ParseGrammarFromString(grammarString);

    Surab::Log("\n===Original Grammar===");
    Surab::Compiler::PrintGrammar(G);

    auto [nonTerminalSet, terminalSet] = Surab::Compiler::IdentifyNonTerminalsAndTerminals(G);

    std::unordered_map<Token_t, SSet_t> FIRST = Surab::Compiler::ComputeFirst(G);
    std::unordered_map<Token_t, SSet_t> FOLLOW = Surab::Compiler::ComputeFollow(G, FIRST, startSymbol);

    Surab::Compiler::ParseTable_t table = Surab::Compiler::ConstructParseTable(G, FIRST, FOLLOW);
    Surab::Log("\n===Parse Table===");
    Surab::Compiler::PrintTable(table);

    std::stack<Token_t> stack;
    stack.push("$");
    stack.push(startSymbol);
    size_t ip = 0;

    Surab::Log("\n===Parsing Steps===");
    PrintSeparator();
    PrintTabularSteps("Stack", "Input", "Action");
    PrintSeparator();

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
        inputStr = Surab::Compiler::Trim(inputStr);

        Surab::Compiler::Token_t X = stack.top();
        Surab::Compiler::Token_t a = inputTokens[ip];

        // End of stack and input
        if (X == a && X == "$") {
            PrintTabularSteps(stackStr, inputStr, "Accept", GREEN);
            break;
        }

        // terminal match
        if (X == a) {
            stack.pop();
            ++ip;
            PrintTabularSteps(stackStr, inputStr, std::format("POP({})", X));
            continue;
        }

        // No Production for non-terminal, error
        if (X != "$" && !nonTerminalSet.contains(X)) {
            PrintSeparator();
            Surab::LogError("Error: unexpected terminal {}", X);
            PrintTabularSteps(stackStr, inputStr, "");
            break;
        }

        // No LL(1) table entry for non-terminal, error
        if (!table.contains(X) || !table.at(X).contains(a)) {
            PrintSeparator();
            Surab::LogError("Error: no table entry for {} with input {}", X, a);
            PrintTabularSteps(stackStr, inputStr, "");
            break;
        }

        Surab::Compiler::Production_t production = table.at(X).at(a);

        std::vector<Token_t> tokenList = Surab::Compiler::TokenizeProduction(production, nonTerminalSet);
        stack.pop();
        PrintTabularSteps(stackStr, inputStr, std::format("{} -> {}", X, tokenList[0]));

        // push rhs in reverse (ignore epsilon)
        if (!(tokenList.size() == 1 && tokenList[0] == EPSILON)) {
            for (auto rit = tokenList.rbegin(); rit != tokenList.rend(); ++rit) {
                stack.push(*rit);
            }
        }
    }
    PrintSeparator();

    Footer();
    return 0;
}