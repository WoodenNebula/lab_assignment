# Questions

### 16. Symbol Table Implementation

Write a C program to implement a single-scope Symbol Table using a dynamic array or linked list
supporting full CRUD (Create, Read, Update, Delete) operations.

#### Requirements:

- Structure Definition: Define a struct Symbol containing fields: char name[32], char type[10], int
  address, and int size.
- Interactive Menu:
  - Choice 1 (Insert): Prompt for symbol name, type, address, and size. Search if the symbol already exists. If present, display 'Duplicate symbol'. Otherwise, insert it.
  - Choice 2 (Display): Print all symbols in a formatted table with columns: Index, Name, Type, Address, Size.
  - Choice 3 (Delete): Search for a symbol by name. If not found, display 'Label not found'. Otherwise, delete the entry and adjust indices.
  - Choice 4 (Search): Search for a symbol by name. Display its index and attributes if found, or 'Symbol not found'.
  - Choice 5 (Modify): Search for a symbol by name. Allow modifying its address, type, or both.
  - Choice 6 (Exit): Terminate the program.

#### Hints & Implementation Details:

Write a helper function int findSymbol(const char\* name) returning array index or -1. Left-shift
subsequent elements upon array deletion.

### 17. Symbol Table Construction and Scope Management

Implement a multi-level scoped symbol table in C++ using a stack of hash map to handle nested scope rules for a block structured language (supporting { ... } blocks).

#### Requirements:

- Symbol Table API:
  - enter_scope(): Pushes a new hash map onto the scope stack.
  - leave_scope(): Pops the top scope hash map off the stack.
  - insert(std::string name, SymbolInfo info): Inserts a variable into the current (top) scope map. If it already exists in the top map, emit 'Redeclaration Error: <name> in current scope'.
- lookup(std::string name): Searches from the top scope down to the global scope (bottom of stack). Returns the symbol or emits 'Undeclared Error: <name>'.
- AST Simulation: Mock an Abstract Syntax Tree (AST) using simple structs representing block entry/exit, variable declarations, and identifier lookups.

#### Hints & Implementation Details:

Scoped Symbol Table Class Structure:

```
struct SymbolInfo {
    std::string name;
    std::string type; // 'int', 'float', etc.
    int size;
    int scopeLevel;
};
class ScopedSymbolTable {
private:
    std::vector<std::unordered_map<std::string, SymbolInfo>> scopeStack;
public:
    ScopedSymbolTable() { enter_scope(); } // Global scope
    void enter_scope() { scopeStack.push_back({}); }
    void leave_scope() { if (!scopeStack.empty()) scopeStack.pop_back(); }
    SymbolInfo* lookup(const std::string& name) {
        for (int i = scopeStack.size() - 1; i >= 0; --i) {
            auto it = scopeStack[i].find(name);
            if (it != scopeStack[i].end()) return &(it->second);
        }
        return nullptr;
    }
};
```

### 18. Type Checking for Expressions

Develop a C++ semantic analysis module that performs type inference, compatibility checks, and implicit type coercions on arithmetic expressions, assignments, and array references.

#### Requirements:

- Type Enumeration: Define enum DataType { TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_ARRAY, TYPE_ERROR }.
- Type Coercion Rules:
  - INT + INT -> INT
  - INT + FLOAT -> FLOAT (Implicit promotion)
  - FLOAT + FLOAT -> FLOAT
  - Incompatible operations (e.g., INT + BOOL) trigger explicit error logs.
- Array Index Verification: Ensure array indices evaluate to TYPE_INT. Check compile-time bounds if array dimensions are stored in the symbol table.
- Error Reporting: Log errors with line numbers and descriptive messages.

#### Hints & Implementation Details:

Binary Expression Type Resolution Logic:

```
DataType evaluate_binary_type(DataType left, DataType right, int lineNo) {
if (left == TYPE_ERROR || right == TYPE_ERROR) return TYPE_ERROR;
if (left == TYPE_INT && right == TYPE_INT) return TYPE_INT;
if ((left == TYPE_INT && right == TYPE_FLOAT) || (left == TYPE_FLOAT && right ==
TYPE_INT)) {
return TYPE_FLOAT;
}
if (left == TYPE_FLOAT && right == TYPE_FLOAT) return TYPE_FLOAT;
std::cout << "Line " << lineNo << ": Type Error - Incompatible operands.\n";
return TYPE_ERROR;
}
```

### Semantic Rules for Control Structures

Implement semantic checks for if-else, while, and for loop control structures in a mini-language using an AST Visitor pattern in C++.

#### Requirements:

_ AST Node Hierarchy: Define an abstract base class ASTNode with a pure virtual accept(SemanticVisitor\*) method. Implement concrete node classes for IfASTNode, WhileASTNode, ForASTNode, BreakASTNode, ContinueASTNode, and expression nodes.
_ Visitor Pattern Architecture: Implement a SemanticVisitor interface containing pure virtual visit methods for each node type to decouple AST structure from semantic analysis rules.
_ Loop Context Tracking: Maintain a loop_depth integer counter within SemanticVisitor to ensure break and continue statements occur strictly within loop constructs (loop_depth > 0). Emit an error if misplaced.
_ Condition Validation: Verify that condition expressions inside if, while, and for evaluate strictly to TYPE\*BOOL (or integer types convertible to boolean).

- Variable Initialization Tracking: Track variable initialization flags (bool is_initialized) in the symbol table to issue warnings or errors for use-before-definition within control blocks

#### Hints & Implementation Details:

Define the AST hierarchy with double-dispatch methods. In the visitor implementation, explicitly govern tree traversal order: evaluate condition nodes first, enforce type constraints, adjust contextual state like loop_depth, visit body sub-trees, and restore state upon exiting.

```
#include <iostream>
#include <vector>
enum DataType { TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_ERROR };
// Forward declarations
class IfASTNode;
class WhileASTNode;
class BreakASTNode;
// Abstract Visitor Interface
class SemanticVisitor {
public:
virtual DataType visitIfNode(IfASTNode* node) = 0;
virtual DataType visitWhileNode(WhileASTNode* node) = 0;
virtual DataType visitBreakNode(BreakASTNode* node) = 0;
};
// Base AST Node
class ASTNode {
public:
int lineNo;
virtual ~ASTNode() = default;
virtual DataType accept(SemanticVisitor* visitor) = 0;
};
// Concrete AST Nodes
class WhileASTNode : public ASTNode {
public:
ASTNode* condition;
ASTNode* body;
WhileASTNode(ASTNode* cond, ASTNode* b, int line)
: condition(cond), body(b) { lineNo = line; }
DataType accept(SemanticVisitor* visitor) override {
return visitor->visitWhileNode(this);
}
};
class BreakASTNode : public ASTNode {
public:
BreakASTNode(int line) { lineNo = line; }
DataType accept(SemanticVisitor* visitor) override {
return visitor->visitBreakNode(this);
}
};
// Concrete Visitor Implementation
class ConcreteSemanticVisitor : public SemanticVisitor {
private:
int loop_depth = 0;
public:
DataType visitWhileNode(WhileASTNode* node) override {
// Evaluate condition expression
DataType condType = node->condition->accept(this);
if (condType != TYPE_BOOL && condType != TYPE_INT) {
std::cout << "Line " << node->lineNo
<< ": Error - Loop condition must evaluate to boolean.\n";
}
// Enter loop context
loop_depth++;
if (node->body) {
node->body->accept(this);
}
// Exit loop context
loop_depth--;
return TYPE_BOOL;
}
DataType visitBreakNode(BreakASTNode* node) override {
// Enforce loop context constraint
if (loop_depth == 0) {
std::cout << "Line " << node->lineNo
<< ": Error - 'break' statement outside loop context.\n";
return TYPE_ERROR;
}
return TYPE_BOOL;
}
DataType visitIfNode(IfASTNode* node) override {
// Handle if-statement semantic validation
return TYPE_BOOL;
}
};
```

### 20. Intermediate Code Generation for Expressions

Write a C++ program to generate Three-Address Code (TAC) in Quadruple format for arithmetic and relational expressions, including operators +, -, \*, /, ==, >, <, unary minus, and parentheses ().

#### Requirements:

- Quadruple Data Structure: Define struct Quadruple { std::string op, arg1, arg2, result; }.
- Parsing Strategy: Parse an infix string using Shunting-Yard algorithm to generate postfix notation or build an AST, then emit quadruples.
- Temporary Variable Generator: Implement std::string new_temp() returning incremental labels (t1, t2, t3...).

#### Hints & Implementation Details:

Postfix to TAC Quadruple Generator:

```
struct Quadruple { std::string op, arg1, arg2, result; };
std::vector<Quadruple> quadList;
int tempCount = 1;
std::string newTemp() { return "t" + std::to_string(tempCount++); }
void generateTACFromPostfix(const std::vector<std::string>& tokens) {
std::stack<std::string> st;
for (const auto& token : tokens) {
if (isOperator(token)) {
std::string arg2 = st.top(); st.pop();
std::string arg1 = st.top(); st.pop();
std::string t = newTemp();
quadList.push_back({token, arg1, arg2, t});
st.push(t);
} else { st.push(token); }
}
}
```

### 21. TAC Generation for Control Flow Statements

Implement intermediate code generation for if-else and while control flow statements in C++ using boolean short-circuit evaluation and backpatching.

#### Requirements:

- Label Generation: Maintain a global counter producing labels (L1, L2, ...).
- Backpatching Utility Functions: make_list(int quad_index), merge_list(list1, list2), and backpatch(list, label).
- Short-Circuit Logical Ops: Handle && and || by emitting conditional jumps (ifFalse cond goto ...).

#### Hints & Implementation Details:

Control Flow Templates:

- if (cond) S1 else S2 -> Evaluate cond -> truelist -> L_then, falselist -> L_else
- while (cond) S -> L_begin: Evaluate cond -> truelist -> L_body, falselist -> L_exit

```
// Backpatch function sketch
void backpatch(const std::vector<int>& list, const std::string& label) {
for (int quadIdx : list) {
quadList[quadIdx].result = label;
}
}
```

### 22. Intermediate Code for Functions and Parameters

Generate TAC quadruples for function declarations, parameter passing (by value), function calls, and return statements in C++.

#### Requirements:

- Standard Opcodes: FUNC_BEGIN func_name, PARAM x, CALL func_name, arg_count, return_temp, RETURN x, FUNC_END func_name.
- Signature Verification: Cross-check caller arguments count against parameter count declared in symbol table.

#### Hints & Implementation Details:

Example Output Sequence for x = foo(a, b):

```
PARAM a
PARAM b
t1 = CALL foo, 2
x = t1 23. Code Generation
```

### 23. Code Generation

Implement a code generator in C++ that translates TAC quadruples into 16-bit 8086 Assembly (MASM/TASM syntax).

### Requirements:

- Target Architecture & Registers: 8086 CPU using registers AX, BX, CX, DX.
- Stack Frame / Local Variables: Access variables and temporaries via Base Pointer offset: [BP - offset].
- Register Allocation & Spilling: Allocate available registers dynamically. Spill to memory using PUSH / MOV [BP - offset], reg when registers are occupied.
- Prologue / Epilogue: Generate standard 8086 frames (PUSH BP, MOV BP, SP, SUB SP, local\*size / MOV SP, BP, POP BP, RET)

### Hints & Implementation Details:

```
TAC Operator to 8086 Instruction Mapping:
// t1 = a + b
MOV AX, [BP - 2]
ADD AX, [BP - 4]
MOV [BP - 6], AX

// t2 = a * b
MOV AX, [BP - 2]
IMUL WORD PTR [BP - 4]
MOV [BP - 8], AX

// if False t1 goto L1
MOV AX, [BP - 6]
CMP AX, 0
JE L1
```

### 24. Constant Folding and Constant Propagation

Write an intermediate code optimizer in C++ for TAC that performs multi-pass Constant Folding and Constant Propagation until reaching a fixed point.

#### Requirements:

- Constant Table: Maintain a std::unordered\*map<std::string, double> constTable.
- Constant Propagation: Replace variable operands with known numerical constants.
- Constant Invalidation Rule: If a variable is assigned a non-constant expression later (e.g., x = y + z), erase x from constTable.
- Constant Folding: Evaluate expressions with constant operands at compile time (e.g., t1 = 4 \* 5 -> t1 = 20).
- Fixed-Point Iteration: Loop optimization passes until an entire pass makes zero modifications.

#### Hints & Implementation Details:

Multi-pass Optimization Algorithm:

```
bool modified = true;
while (modified) {
modified = false;
for (auto& quad : quadList) {
if (constTable.count(quad.arg1)) { quad.arg1 =
std::to_string(constTable[quad.arg1]); modified = true; }
if (constTable.count(quad.arg2)) { quad.arg2 =
std::to_string(constTable[quad.arg2]); modified = true; }
if (isNumber(quad.arg1) && isNumber(quad.arg2) && quad.op == "+") {
double res = std::stod(quad.arg1) + std::stod(quad.arg2);
quad.op = "="; quad.arg1 = std::to_string(res); quad.arg2 = "";
constTable[quad.result] = res;
modified = true;
}
}
}
```

### 25. Dead Code Elimination and Control Flow Graph

Implement a C++ optimization pass that partitions TAC into Basic Blocks, builds a Control Flow Graph (CFG), and performs liveness analysis to eliminate unreachable code and dead assignments.

#### Requirements:

- Leader Identification: Identify basic block leaders (first instruction, target of jump, instruction following jump).
- CFG Construction: Represent basic blocks as nodes and jumps as directed edges in an adjacency list.
- Live Roots & Backward Analysis: Identify live roots (RETURN statements, global writes, I/O ops). Perform backward sweep to flag live instructions and purge unmarked instructions.

#### Hints & Implementation Details:

Basic Block Leader Example:

```
(1) i = 1 <-- Leader (First instruction)
(2) L1: t1 = i \* 4 <-- Leader (Target of jump)
(3) if t1 > 100 goto L2
(4) i = i + 1 <-- Leader (Follows jump)
(5) goto L1
(6) L2: return i <-- Leader (Target of jump)
```

### 26. Peephole Optimization

Develop a C/C++ program to apply sliding window (size 2-3 instructions) Peephole Optimizations on generated 8086 Assembly code.

#### Requirements:

- Redundant Load/Store Elimination: MOV AX, [BP-2] followed by MOV [BP-2], AX -> Eliminate second line.
- Algebraic Identities: ADD AX, 0, SUB AX, 0, or MUL AX, 1 -> Eliminate. MUL AX, 0 -> MOV AX, 0.
- Strength Reduction: IMUL AX, 2 -> SHL AX, 1.
- Redundant Jump Elimination: JMP L1 followed directly by L1: -> Eliminate jump.
- Unreachable Code Removal: Delete instructions directly following unconditional JMP up to next label.

#### Hints & Implementation Details:

Sliding Window Sweeper:

```
void applyPeephole(std::vector<std::string>& instrs) {
bool changed = true;
while (changed) {
changed = false;
for (size_t i = 0; i < instrs.size() - 1; ++i) {
std::string src1, dest1, src2, dest2;
if (parseMov(instrs[i], dest1, src1) && parseMov(instrs[i+1], dest2, src2))
{
if (dest1 == src2 && src1 == dest2) {
instrs.erase(instrs.begin() + i + 1);
changed = true;
}
}
}
}
}
```
