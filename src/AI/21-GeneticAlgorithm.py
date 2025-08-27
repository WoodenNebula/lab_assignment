# ------------------------------------------------------------
# Genetic Algorithm Example: Maximize f(x) = x^2
#
# Genetic Algorithms (GA) are search and optimization techniques
# inspired by natural evolution. They use mechanisms like selection,
# crossover, and mutation to evolve solutions over generations.
#
# Steps:
#   1. Initialize a population of candidate solutions.
#   2. Evaluate fitness of each individual.
#   3. Select parents based on fitness.
#   4. Apply crossover to produce offspring.
#   5. Apply mutation to maintain diversity.
#   6. Repeat for multiple generations.
#
# Applications: Optimization, scheduling, machine learning, AI problem-solving.
# ------------------------------------------------------------

import os
import random
from typing import List, Tuple

# Parameters
POP_SIZE = 6
CHROM_LENGTH = 5  # Binary string length to represent numbers 0-31
GENS = 5
MUTATION_RATE = 0.1

def binary_to_int(chrom: str) -> int:
    return int(chrom, 2)

def fitness(chrom: str) -> int:
    x = binary_to_int(chrom)
    return x * x

def create_population() -> List[str]:
    return [''.join(random.choice('01') for _ in range(CHROM_LENGTH)) for _ in range(POP_SIZE)]

def select_pair(pop: List[str]) -> Tuple[str, str]:
    total_fitness = sum(fitness(c) for c in pop)
    probs = [fitness(c)/total_fitness for c in pop]
    return random.choices(pop, weights=probs, k=2)

def crossover(p1: str, p2: str) -> Tuple[str, str]:
    point = random.randint(1, CHROM_LENGTH-1)
    return p1[:point]+p2[point:], p2[:point]+p1[point:]

def mutate(chrom: str) -> str:
    chrom_list = list(chrom)
    for i in range(len(chrom_list)):
        if random.random() < MUTATION_RATE:
            chrom_list[i] = '1' if chrom_list[i] == '0' else '0'
    return ''.join(chrom_list)

def main() -> None:
    for i in range(2):
        print(f"{"-" * 20} Run-{i+1}{"-" * 20}")
        pop = create_population()
        print("Initial Population:", pop)
        for gen in range(1, GENS+1):
            new_pop = []
            while len(new_pop) < POP_SIZE:
                p1, p2 = select_pair(pop)
                c1, c2 = crossover(p1, p2)
                c1, c2 = mutate(c1), mutate(c2)
                new_pop.extend([c1, c2])
            pop = new_pop[:POP_SIZE]
            best = max(pop, key=fitness)
            print(f"Generation {gen}: Best={best} ({binary_to_int(best)}), Fitness={fitness(best)}")
    
        print("Final Population:", pop)
        best = max(pop, key=fitness)
        print("Best Solution:", best, "=", binary_to_int(best))
    
    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
