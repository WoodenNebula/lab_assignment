# ------------------------------------------------------------
# Simple Backpropagation Learning Example
#
# Backpropagation is a supervised learning algorithm for training
# artificial neural networks. It minimizes the error by propagating
# the output error backwards and updating the weights using
# gradient descent.
#
# Process:
#   - Forward pass: compute network output for given inputs.
#   - Compute error: difference between predicted and target output.
#   - Backward pass: propagate error to update weights.
#
# Applications: Pattern recognition, function approximation, AI learning.
# ------------------------------------------------------------

import os
import math
from typing import List, Tuple

def sigmoid(x: float) -> float:
    return 1 / (1 + math.exp(-x))

def sigmoid_derivative(x: float) -> float:
    return x * (1 - x)

class SimpleNN:
    def __init__(self) -> None:
        # Initialize weights and biases
        self.w_input_hidden = [[0.5, -0.4], [0.3, 0.1]]  # 2x2 weights
        self.b_hidden = [0.0, 0.0]
        self.w_hidden_output = [0.2, -0.3]  # 1x2 weights
        self.b_output = 0.0
        self.learning_rate = 0.5

    def forward(self, x: List[float] | List[int]) -> float:
        self.h_input = [
            x[0]*self.w_input_hidden[0][0] + x[1]*self.w_input_hidden[1][0] + self.b_hidden[0],
            x[0]*self.w_input_hidden[0][1] + x[1]*self.w_input_hidden[1][1] + self.b_hidden[1],
        ]
        self.h_output = [sigmoid(h) for h in self.h_input]
        self.o_input = sum(h*w for h, w in zip(self.h_output, self.w_hidden_output)) + self.b_output
        self.o_output = sigmoid(self.o_input)
        return self.o_output

    def train(self, x: List[float] | List[int], y: float) -> None:
        # Forward pass
        output = self.forward(x)
        # Output layer error
        error = y - output
        delta_output = error * sigmoid_derivative(output)
        # Hidden layer error
        delta_hidden = [
            delta_output * self.w_hidden_output[i] * sigmoid_derivative(self.h_output[i])
            for i in range(2)
        ]
        # Update weights hidden->output
        for i in range(2):
            self.w_hidden_output[i] += self.learning_rate * delta_output * self.h_output[i]
        self.b_output += self.learning_rate * delta_output
        # Update weights input->hidden
        for i in range(2):
            self.w_input_hidden[0][i] += self.learning_rate * delta_hidden[i] * x[0]
            self.w_input_hidden[1][i] += self.learning_rate * delta_hidden[i] * x[1]
            self.b_hidden[i] += self.learning_rate * delta_hidden[i]

def main() -> None:
    nn = SimpleNN()
    training_data = [
        ([0, 0], 0),
        ([0, 1], 1),
        ([1, 0], 1),
        ([1, 1], 0),
    ]  # Learning XOR for demo
    # Train for 1000 epochs
    for epoch in range(1000):
        for x, y in training_data:
            nn.train(x, y)

    # Test examples
    test_cases = [[0, 0], [1, 1]]
    for i, x in enumerate(test_cases, 1):
        print(f"Test case {i}: Input {x} -> Output {nn.forward(x):.4f}")

    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
