# ------------------------------------------------------------
# Water Jug Problem
#
# The Water Jug problem is a classic AI search problem that can be
# solved using state-space search techniques like BFS or DFS.
#
# Problem:
#   - Two jugs of different capacities (4-gallon and 3-gallon) with no
#     measuring markers.
#   - Goal: Get exactly 2 gallons in the 4-gallon jug.
#
# Approach:
#   - Represent states as (amount_in_4_gal, amount_in_3_gal).
#   - Use BFS to explore all possible actions:
#       Fill a jug, empty a jug, pour one jug into another.
#   - Stop when the goal state is reached.
#
# Applications: Constraint satisfaction, planning, AI problem-solving.
# ------------------------------------------------------------

import os
from collections import deque
from typing import Tuple, List, Set

def get_successors(state: Tuple[int, int]) -> List[Tuple[int, int]]:
    jug4, jug3 = state
    successors = []

    # Fill actions
    successors.append((4, jug3))  # Fill 4-gal
    successors.append((jug4, 3))  # Fill 3-gal

    # Empty actions
    successors.append((0, jug3))  # Empty 4-gal
    successors.append((jug4, 0))  # Empty 3-gal

    # Pour 4->3
    pour = min(jug4, 3 - jug3)
    successors.append((jug4 - pour, jug3 + pour))

    # Pour 3->4
    pour = min(jug3, 4 - jug4)
    successors.append((jug4 + pour, jug3 - pour))

    return successors

def bfs_solution():
    start = (0, 0)
    goal = 2
    queue = deque([(start, [start])])
    visited: Set[Tuple[int, int]] = set()
    
    while queue:
        state, path = queue.popleft()
        if state in visited:
            continue
        visited.add(state)
        if state[0] == goal:
            return path
        for succ in get_successors(state):
            if succ not in visited:
                queue.append((succ, path + [succ]))
    return []

def main() -> None:
    path = bfs_solution()
    if path:
        print("Steps to get 2 gallons in 4-gallon jug:")
        for step in path:
            print(f"4-gal: {step[0]}, 3-gal: {step[1]}")
    else:
        print("No solution found.")

    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
