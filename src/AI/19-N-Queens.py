# ------------------------------------------------------------
# N-Queens Problem
#
# The N-Queens problem is a classic example in Artificial Intelligence
# and combinatorial search. The goal is to place N queens on an N×N
# chessboard such that no two queens threaten each other.
#
# Backtracking is an AI search technique:
#   - Place a queen in a row and recursively attempt to place the next queen.
#   - If a conflict arises, backtrack and try a different column.
#   - Continue until a solution is found or all options are exhausted.
#
# Applications: Constraint satisfaction problems, scheduling, puzzle solving.
# ------------------------------------------------------------

import os
from typing import List

def is_safe(board: List[int], row: int, col: int) -> bool:
    """Check if placing a queen at (row, col) is safe."""
    for r in range(row):
        c = board[r]
        if c == col or abs(c - col) == abs(r - row):
            return False
    return True

def solve_n_queens_util(board: List[int], row: int, N: int) -> bool:
    if row == N:
        return True  # All queens placed successfully
    for col in range(N):
        if is_safe(board, row, col):
            board[row] = col
            if solve_n_queens_util(board, row + 1, N):
                return True
            board[row] = -1  # Backtrack
    return False

def solve_n_queens(N: int) -> List[int]:
    """Return one solution as a list where index = row, value = column."""
    board = [-1] * N
    if solve_n_queens_util(board, 0, N):
        return board
    else:
        return []

def print_board(board: List[int]) -> None:
    N = len(board)
    for row in range(N):
        line = ""
        for col in range(N):
            if board[row] == col:
                line += "Q "
            else:
                line += ". "
        print(line)
    print("-" * 40)

def main() -> None:
    N: int = int(input("Enter the value of N: "))
    solution = solve_n_queens(N)
    if solution:
        print(f"One solution for N={N}:")
        print_board(solution)
    else:
        print(f"No solution exists for N={N}")

    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
