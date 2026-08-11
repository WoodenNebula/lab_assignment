# Questions

> Write programs in C++ unless specified otherwise

### 5. Write a program to simulate lexical analyzer for validating operators. Write another program to implement a lexical analyzer pass that scans an input string or file and identifies, validates, and classifies all operators.

- Supported Categories:
  - Arithmetic: +, -, \*, /, %
  - Relational: <, >, <=, >=, ==, !=
  - Logical: &&, ||, !
  - Assignment: =, +=, -=, \*=, /=
- Requirements:
  - The scanner must correctly distinguish between single-character operators (e.g., < or =) and multi-character operators (e.g., <= or ==) using lookahead techniques.
  - Output a structured list of valid operators found in the input format: <OPERATOR_LEXEME, CATEGORY, LINE_NUMBER>.
  - Print an explicit error message (e.g., "Invalid Operator: &") if an incomplete or malformed token sequence is encountered

### 6. Lexical Analysis via Automated Tool (FLEX)

Write a Flex specification file (.l) to construct a lexical analyzer for a C subset.

- Define regular expressions for keywords, identifiers, integer/float constants, arithmetic operators, and delimiters.
- Skip whitespace and comments, maintain line counters, and output tokens as <TOKEN_TYPE, lexeme, line_no>.

### 7. Top-Down Parsing Grammar Transformations

Write a program to preprocess a Context-Free Grammar (CFG):

- Eliminate direct and indirect Left Recursion.
- Apply Left Factoring on productions sharing common prefixes.

### 8. FIRST and FOLLOW Computation

Write a program to read a non-recursive Context-Free Grammar and compute the FIRST and FOLLOW sets for grammar symbols.

### 9. Constructing LL(1) Parsing Table

Write a program that takes an arbitrary, non-left-recursive Context-Free Grammar (CFG) as input and constructs its LL(1) Parsing Table. <br>
[Print the final FIRST, FOLLOW, and formatted 2D LL(1) Parsing Table in tabular form]

### 10. Write a program to implement top-down non-recursive descent parsing for a grammar (assuming some grammar and LL(1) parsing table)

### 11. Implementing Shift Reduce Parser

Write a program for implementing shift reduce parser. The program should do the followings:

- get the input expression and store it in the input buffer
- read the data from the input buffer one at the time.
- using stack and push and pop operation, shift and reduce symbols with respect to production rules available.
- Continue the process till symbol shift and production rule reduce reaches the start symbol.
- Display formatted parsing table showing Stack Contents, Input Buffer, and Action Taken at each step

### 12. Constructing SLR Parsing Table

Write a C/C++ program to compute LR(0) canonical items and construct the SLR(1) Parsing Table for a given Context-Free Grammar.

    S→ E
    S→ E+T|T
    T→T\*F|F
    F→(E) |id

Instructions:

- implement core functions like CLOSURE(I) and GOTO(I,X)
- Generate the complete set of canonical LR(0) collection states C = {I0, I1, …., In}
- compute SLR(1) table actions
- print the complete ACTION and GOTO table in tabular form.

### 13. LR(1) Parsing Table Construction

Write a program to generate Canonical LR(1) items and construct the LR(1) parsing Table for a given Context-Free Grammar. Print all generated LR(1) state sets with lookaheads, display the formatted ACTION and GOTO tables.

### 14. LALR(1) Parsing Table Construction

Write a program that merges identical LR(1) cores to construct a Look-Ahead LR (LALR(1)) Parsing Table for above case.

### 15. LALR(1) Parsing via Automated Tool (BISON / YACC)

Construct a parser using Flex and Bison for variable declaration statements and arithmetic expressions:

- Define Context-Free Grammar rules inside a .y file.
- Connect Bison parser with Flex scanner via yylex().
- Resolve shift/reduce ambiguities using operator precedence declarations (%left, %right).
