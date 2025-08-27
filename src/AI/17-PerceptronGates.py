# ------------------------------------------------------------
# Realization of AND, OR, NOT gates using Artificial Neurons
#
# In AI, an artificial neuron (or perceptron) models a simple
# biological neuron. It computes a weighted sum of inputs and
# passes it through an activation function (here, a step function).
#
# Logic gates (AND, OR, NOT) can be realized using a single
# neuron by carefully choosing weights and bias.
#
# - AND: fires only if all inputs are 1
# - OR: fires if at least one input is 1
# - NOT: inverts a single input
#
# This illustrates how simple neural networks can perform logical reasoning.
# ------------------------------------------------------------

import os
from typing import List

def neuron(inputs: List[int], weights: List[int], bias: int) -> int:
    """Single artificial neuron using step activation function."""
    total = sum(i*w for i, w in zip(inputs, weights)) + bias
    return 1 if total > 0 else 0

def AND_gate(x1: int, x2: int) -> int:
    return neuron([x1, x2], weights=[1, 1], bias=-1)

def OR_gate(x1: int, x2: int) -> int:
    return neuron([x1, x2], weights=[1, 1], bias=0)

def NOT_gate(x: int) -> int:
    return neuron([x], weights=[-1], bias=0)

def main() -> None:
    print("AND Gate")
    for x1 in [0, 1]:
        for x2 in [0, 1]:
            print(f"{x1} AND {x2} = {AND_gate(x1, x2)}")
    print("-" * 40)

    print("OR Gate")
    for x1 in [0, 1]:
        for x2 in [0, 1]:
            print(f"{x1} OR {x2} = {OR_gate(x1, x2)}")
    print("-" * 40)

    print("NOT Gate")
    for x in [0, 1]:
        print(f"NOT {x} = {NOT_gate(x)}")
    print("-" * 40)

    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
