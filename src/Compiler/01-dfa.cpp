/*
Implement DFA over alphabet {0,1} such that, it accepts all the strings that:
a. start with 01
b. end with 01
c. contains 001 as substring
*/

#include "src/commons.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <print>


class DFA {
public:
    std::string name;
    constexpr static int AcceptingState{ INT32_MAX }; // Arbitrary large number to represent accepting state
    constexpr static int RejectedState{ -1 };

public:
    DFA(const std::string& n) : name(n) {}

    virtual int Delta(int state, char input) const = 0;
};

class DFAStarting : public DFA {
public:
    DFAStarting() : DFA("DFA Starting with 01") {}

    int Delta(int state, char input) const override {
        switch (state) {
        case 0:
            if (input == '0') return 1;
            else return RejectedState;
        case 1:
            if (input == '1') return AcceptingState;
            else return RejectedState;
        case AcceptingState:
            return AcceptingState;
        case RejectedState:
        default:
            return RejectedState;
        }
    }
};

class DFAEnding : public DFA {
public:
    DFAEnding() : DFA("DFA Ending with 01") {}

    int Delta(int state, char input) const override {
        switch (state) {
        case 0:
            if (input == '0') return 1;
            else return 0;
        case 1:
            if (input == '1') return AcceptingState;
            else return 1;
        case AcceptingState:
            if (input == '0') return 1;
            else return 0;
        default:
            return RejectedState;
        }
    }
};


class DFASubstring : public DFA {
public:
    DFASubstring() : DFA("DFA Containing 001") {}

    int Delta(int state, char input) const override {
        switch (state) {
        case 0:
            if (input == '0') return 1;
            else return 0;
        case 1:
            if (input == '0') return 2;
            else return 0;
        case 2:
            if (input == '1') return AcceptingState;
            else return 2;
        case AcceptingState:
            return AcceptingState;
        default:
            return RejectedState;
        }
    }
};




bool ExecDFA(const DFA& dfa, const std::string& input, const std::vector<char>& alphabet) {
    int state = 0;

    for (char c : input) {
        // validate input character
        if (std::find(alphabet.begin(), alphabet.end(), c) == alphabet.end()) {
            std::print("\nInvalid character {} in input string. Only 0 and 1 are allowed.", c);
            return false;
        }

        state = dfa.Delta(state, c);
        // Early return, might not need at all
        if (state == dfa.RejectedState) {
            return false;
        }
    }

    return (state == dfa.AcceptingState);
}

std::string GetInput() {
    std::string input;
    std::print("Enter a binary string: ");
    std::cin >> input;
    return input;
}

int main() {
    Header("DFA Implementation");

    std::string input = GetInput();
    std::vector<char> alphabet = { '0', '1' };

    DFAStarting dfaStarting;
    DFAEnding dfaEnding;
    DFASubstring dfaSubstring;

    std::vector<DFA*> dfas = { &dfaStarting, &dfaEnding, &dfaSubstring };

    for (const DFA* dfa : dfas) {
        bool isAccepted = ExecDFA(*dfa, input, alphabet);

        if (isAccepted) {
            std::print("\n{}The string {} is accepted by {}{}", GREEN, input, dfa->name, RESET);
        }
        else {
            std::print("\n{}The string {} is rejected by {}{}", RED, input, dfa->name, RESET);
        }
    }

    Footer();
}