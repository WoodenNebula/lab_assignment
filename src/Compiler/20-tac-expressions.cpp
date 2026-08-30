#include "src/commons.hpp"

#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

int tempCount = 1;

std::string newTemp() {
    return "t" + std::to_string(tempCount++);
}

bool IsOperator(const std::string& token) {
    static const std::vector<std::string> ops = { "+", "-", "*", "/", "==", ">", "<", "&&", "||", "!" };
    return std::find(ops.begin(), ops.end(), token) != ops.end();
}

int Precedence(const std::string& op) {
    if (op == "||") return 1;
    if (op == "&&") return 2;
    if (op == "==") return 3;
    if (op == ">" || op == "<") return 4;
    if (op == "+" || op == "-") return 5;
    if (op == "*" || op == "/") return 6;
    if (op == "!") return 7;
    return 0;
}

std::vector<std::string> Tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string current;
    for (char ch : expr) {
        if (std::isspace(static_cast<unsigned char>(ch))) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }

        if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_') {
            current += ch;
        }
        else if (ch == '(' || ch == ')' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' || ch == '<' || ch == '=' || ch == '&' || ch == '|') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            if (ch == '&' || ch == '|') {
                std::string two = std::string(1, ch);
                if (!tokens.empty() && tokens.back() == two) {
                    tokens.back() += ch;
                }
                else {
                    tokens.push_back(std::string(1, ch));
                }
            }
            else if (ch == '=') {
                tokens.push_back("==");
            }
            else {
                tokens.push_back(std::string(1, ch));
            }
        }
    }

    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

std::vector<std::string> InfixToPostfix(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> operators;

    for (const auto& token : tokens) {
        if (token == "(") {
            operators.push(token);
        }
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop();
        }
        else if (IsOperator(token)) {
            while (!operators.empty() && operators.top() != "(" && Precedence(operators.top()) >= Precedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
        else {
            output.push_back(token);
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

std::vector<Quadruple> GenerateTACFromPostfix(const std::vector<std::string>& postfix) {
    std::vector<Quadruple> quads;
    std::stack<std::string> values;

    for (const auto& token : postfix) {
        if (IsOperator(token)) {
            std::string arg2 = values.top(); values.pop();
            std::string arg1 = values.top(); values.pop();
            std::string result = newTemp();
            quads.push_back({ token, arg1, arg2, result });
            values.push(result);
        }
        else {
            values.push(token);
        }
    }

    return quads;
}

void PrintQuadruples(const std::vector<Quadruple>& quads) {
    for (size_t i = 0; i < quads.size(); ++i) {
        const auto& q = quads[i];
        Surab::Log("{}: ({}, {}, {}) -> {}", i + 1, q.op, q.arg1, q.arg2, q.result);
    }
}

int main() {
    Header("TAC for Expressions");

    const std::string expression = "a + b * (c - d)";
    const auto tokens = Tokenize(expression);
    const auto postfix = InfixToPostfix(tokens);
    const auto quads = GenerateTACFromPostfix(postfix);

    Surab::Log("Expression: {}", expression);
    Surab::Log("Tokens: {}", Surab::ToString(tokens, ", ", true));
    Surab::Log("Postfix: {}", Surab::ToString(postfix, ", ", true));
    PrintQuadruples(quads);

    Footer();
    return 0;
}
