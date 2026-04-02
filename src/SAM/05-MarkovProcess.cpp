#include <iostream>
#include <vector>

#include "src/commons.hpp"

int main() {
    Header("Markov Process Simulation");

    Surab::Mat transitionMatrix = {
        { 0.1, 0.6 }, // From state 0 to states 0, 1
        { 0.4, 0.4 }  // From state 1 to states 0, 1
    };

    std::vector<double> initialState = { 1.0, 0.0 };
    std::vector<double> currentState = initialState;

    std::cout << "Initial State: " << Surab::ToString(initialState) << "\n";
    std::cout << "Transition Matrix:\n" << transitionMatrix << "\n";

    std::cout << "--- Simulating Markov Process ---" << std::endl;

    // Simulate the Markov process for a number of steps
    int steps = 10;

    for (int iteration = 0; iteration < steps; iteration++) {

        for (int row = 0; row < transitionMatrix.size(); row++) {
            double nextStateValue = 0.0;
            for (int col = 0; col < transitionMatrix[row].size(); col++) {
                nextStateValue += currentState[col] * transitionMatrix[col][row];
            }
            currentState[row] = nextStateValue;
        }

        std::cout << "[" << iteration + 1 << "]: " << Surab::ToString(currentState) << "\n";
    }

    Footer();
}