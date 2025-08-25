from itertools import permutations

def solve_cryptarithmetic():
    letters = 'TWOFOUR'
    for perm in permutations(range(10), len(set(letters))):
        assign = dict(zip(set(letters), perm))
        
        # Skip numbers with leading zero
        if assign['T'] == 0 or assign['F'] == 0:
            continue

        # Construct numbers
        TWO = assign['T'] * 100 + assign['W'] * 10 + assign['O']
        FOUR = (assign['F'] * 1000 +
                assign['O'] * 100 +
                assign['U'] * 10 +
                assign['R'])

        if TWO + TWO == FOUR:
            print(f"  {TWO}\n+ {TWO}\n= {FOUR}")
            print("Solution:")
            for letter in sorted(assign):
                print(f"{letter} = {assign[letter]}")
            return

    print("No solution found.")


def __main__():
    solve_cryptarithmetic()

    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139 Lab: 1")

__main__()