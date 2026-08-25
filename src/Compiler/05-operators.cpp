/*
Lexical analyzer for validating and classifying operators.
Outputs tokens as: <OPERATOR_LEXEME, CATEGORY, LINE_NUMBER>
Supports arithmetic, relational, logical, and assignment operators.
*/

#include "src/commons.hpp"

#include <string>
#include <vector>
#include <map>

enum class ELexemeCategory { Arithmetic, Relational, Logical, Assignment, Invalid };
std::string CategoryToString(ELexemeCategory cat) {
    switch (cat) {
    case ELexemeCategory::Arithmetic: return "Arithmetic";
    case ELexemeCategory::Relational: return "Relational";
    case ELexemeCategory::Logical: return "Logical";
    case ELexemeCategory::Assignment: return "Assignment";
    default: return "Invalid";
    }
}

struct OpInfo { std::string lexeme; ELexemeCategory category; int line; };
struct LookAheadResult { bool isValid; char nextChar; };

// map of valid second character to allowed first characters for two-character operators
// key: second character(next), value: string of valid first characters
inline const std::map<char, std::string_view> PAIR_OPERATOR_VALID_ANCESTORS = {
    { '&', "&" },
    { '|', "|" },
    { '=', "=!<>+-*/" },
};

bool IsOperator(char c) {
    static const std::string starts = "+-*/%<>=!&|";
    return starts.find(c) != std::string::npos;
}

LookAheadResult LookAheadForSecondOp(const std::string& line, size_t index) {
    if (index + 1 < line.size()) {
        char next = line.at(index + 1);
        if (IsOperator(next)) {
            return { true, next };
        }
    }
    return { false, '\0' };
}

ELexemeCategory ClassifyLexeme(const std::string& lexeme) {
    if (lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "%") return ELexemeCategory::Arithmetic;
    if (lexeme == "<" || lexeme == ">" || lexeme == "<=" || lexeme == ">=" || lexeme == "==" || lexeme == "!=") return ELexemeCategory::Relational;
    if (lexeme == "&&" || lexeme == "||" || lexeme == "!") return ELexemeCategory::Logical;
    if (lexeme == "=" || lexeme == "+=" || lexeme == "-=" || lexeme == "*=" || lexeme == "/=") return ELexemeCategory::Assignment;
    return ELexemeCategory::Invalid;
}

int main(int argc, char** argv) {
    Header("Operators Lexical Analyzer");

    std::vector<OpInfo> collectedOperators;
    std::string lineBuffer;
    int lineNo = 0;

    Surab::Log("Enter text (type ':q' to finish):");
    while (std::getline(std::cin, lineBuffer)) {
        ++lineNo;
        if (lineBuffer.contains(":q")) {
            Surab::Log("Exiting input on line {} due to ':q' command.", lineNo);
            break;
        }
        for (size_t i = 0; i < lineBuffer.size(); ++i) {
            char c = lineBuffer[i];

            // skip loop on non-operator characters
            if (!IsOperator(c))
                continue;

            std::string lexeme;
            lexeme.push_back(c);

            // lookahead
            const auto& [isValid, nextChar] = LookAheadForSecondOp(lineBuffer, i);
            if (isValid) {
                // handle two-char operators
                const auto itr = PAIR_OPERATOR_VALID_ANCESTORS.find(nextChar);

                if (itr != PAIR_OPERATOR_VALID_ANCESTORS.end()) {
                    const auto& [_, validAncestors] = *itr;
                    if (validAncestors.find(c) != std::string::npos) {
                        lexeme.push_back(nextChar);
                        ++i; // consume the next character
                    }
                    else {
                        // invalid two-character operator
                        Surab::LogError("Invalid operator sequence: {}{} at line {}", c, nextChar, lineNo);
                        continue;
                    }
                }
                else {
                    // next character is an operator but not a valid two-character operator
                    Surab::LogError("Invalid two-character operator sequence: {}{} at line {}", c, nextChar, lineNo);
                    lexeme.pop_back(); // remove the invalid next character
                    break;
                }
            }

            ELexemeCategory category = ClassifyLexeme(lexeme);
            if (category == ELexemeCategory::Invalid) {
                Surab::LogError("Invalid Operator: {} at line {}", lexeme, lineNo);
                continue;
            }

            collectedOperators.push_back({ lexeme, category, lineNo });
        }
    }

    if (collectedOperators.empty()) {
        Surab::LogError("No operators found.");
    }
    else {
        Surab::LogSuccess("\nOperators found:");
        for (const auto& o : collectedOperators) {
            Surab::Log("[ {}, {}, {} ]", o.lexeme, CategoryToString(o.category), o.line);
        }
    }

    Footer();
    return 0;
}
