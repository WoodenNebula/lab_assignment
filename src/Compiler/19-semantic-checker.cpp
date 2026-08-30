#include "src/commons.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_ERROR
};

std::string ToString(DataType type) {
    switch (type) {
    case TYPE_INT: return "int";
    case TYPE_FLOAT: return "float";
    case TYPE_BOOL: return "bool";
    case TYPE_ERROR: return "error";
    default: return "unknown";
    }
}

struct SymbolInfo {
    std::string Name;
    DataType Type = TYPE_INT;
    bool bIsInitialized = false;
};

class ISemanticVisitor;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual DataType Accept(ISemanticVisitor* visitor) = 0;
    virtual std::string ToString() const = 0;
public:
    int LineNo = 0;
};

class LiteralNode : public ASTNode {
public:
    LiteralNode(DataType t, const std::string& v, int line) : Type(t), Value(v) { this->LineNo = line; }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override { return "Literal(" + Value + ")"; }

public:
    DataType Type;
    std::string Value;
};

class VariableNode : public ASTNode {
public:
    VariableNode(const std::string& n, int line) : Name(n) { this->LineNo = line; }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override { return "Variable(" + Name + ")"; }
public:
    std::string Name;
};

class BinaryExprNode : public ASTNode {
public:
    BinaryExprNode(const std::string& o, ASTNode* l, ASTNode* r, int line)
        : op(o), Left(std::move(l)), Right(std::move(r)) {
        this->LineNo = line;
    }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override { return "BinaryExpr(" + Left->ToString() + " " + op + " " + Right->ToString() + ")"; }
public:
    std::string op;
    ASTNode* Left;
    ASTNode* Right;
};

class IfNode : public ASTNode {
public:
    IfNode(ASTNode* cond, ASTNode* thenBody, ASTNode* elseBody, int line)
        : Condition(std::move(cond)), ThenBranch(std::move(thenBody)), ElseBranch(std::move(elseBody)) {
        this->LineNo = line;
    }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override {
        std::string result = "If(" + Condition->ToString() + ")";
        if (ThenBranch) {
            result += " Then{ " + ThenBranch->ToString() + " }";
        }
        if (ElseBranch) {
            result += " Else{ " + ElseBranch->ToString() + " }";
        }
        return result;
    }
public:
    ASTNode* Condition;
    ASTNode* ThenBranch;
    ASTNode* ElseBranch;
};

class WhileNode : public ASTNode {
public:
    WhileNode(ASTNode* cond, ASTNode* bodyStmt, int line)
        : Condition(std::move(cond)), Body(std::move(bodyStmt)) {
        this->LineNo = line;
    }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override { return "While(" + Condition->ToString() + ") { " + Body->ToString() + " }"; }
public:
    ASTNode* Condition;
    ASTNode* Body;
};

class BreakNode : public ASTNode {
public:
    BreakNode(int line) { this->LineNo = line; }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override { return "Break"; }
};

class ContinueNode : public ASTNode {
public:
    ContinueNode(int line) { this->LineNo = line; }
    DataType Accept(ISemanticVisitor* visitor) override;
    std::string ToString() const override { return "Continue"; }
};

class ISemanticVisitor {
public:
    virtual ~ISemanticVisitor() = default;
    virtual DataType VisitLiteral(LiteralNode* node) = 0;
    virtual DataType VisitVariable(VariableNode* node) = 0;
    virtual DataType VisitBinaryExpr(BinaryExprNode* node) = 0;
    virtual DataType VisitIf(IfNode* node) = 0;
    virtual DataType VisitWhile(WhileNode* node) = 0;
    virtual DataType VisitBreak(BreakNode* node) = 0;
    virtual DataType VisitContinue(ContinueNode* node) = 0;
};

class SemanticAnalyzer : public ISemanticVisitor {
public:
    DataType VisitLiteral(LiteralNode* node) override { return node->Type; }

    DataType VisitVariable(VariableNode* node) override {
        auto it = m_Symboltable.find(node->Name);
        if (it == m_Symboltable.end()) {
            Surab::LogError("Line {}: Undeclared variable '{}'", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }

        if (!it->second.bIsInitialized) {
            Surab::LogError("Line {}: Variable '{}' used before initialization.", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }

        return it->second.Type;
    }

    DataType VisitBinaryExpr(BinaryExprNode* node) override {
        const DataType leftType = node->Left->Accept(this);
        const DataType rightType = node->Right->Accept(this);

        if (leftType == TYPE_ERROR || rightType == TYPE_ERROR) {
            return TYPE_ERROR;
        }

        if (node->op == "+" || node->op == "-" || node->op == "*" || node->op == "/") {
            if (leftType == TYPE_INT && rightType == TYPE_INT)
                return TYPE_INT;
            if ((leftType == TYPE_INT && rightType == TYPE_FLOAT)
                || (leftType == TYPE_FLOAT && rightType == TYPE_INT)
                || (leftType == TYPE_FLOAT && rightType == TYPE_FLOAT)
                ) {
                return TYPE_FLOAT;
            }
            Surab::LogError("Line {}: Incompatible operands for '{}' at `{}`", node->LineNo, node->op, node->ToString());
            return TYPE_ERROR;
        }

        if (node->op == "==" || node->op == "<" || node->op == ">") {
            if (leftType == rightType) return TYPE_BOOL;
            Surab::LogError("Line {}: Comparison between incompatible types at `{}`", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }

        Surab::LogError("Line {}: Unsupported operator '{}' at `{}`", node->LineNo, node->op, node->ToString());
        return TYPE_ERROR;
    }

    DataType VisitIf(IfNode* node) override {
        DataType condType = node->Condition->Accept(this);
        if (condType != TYPE_BOOL && condType != TYPE_INT) {
            Surab::LogError("Line {}: If-condition must be boolean or int, at `{}`", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }

        if (node->ThenBranch) { node->ThenBranch->Accept(this); }
        if (node->ElseBranch) { node->ElseBranch->Accept(this); }
        return TYPE_BOOL;
    }

    DataType VisitWhile(WhileNode* node) override {
        DataType condType = node->Condition->Accept(this);
        if (condType != TYPE_BOOL && condType != TYPE_INT) {
            Surab::LogError("Line {}: While-condition must be boolean or int, at `{}`", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }

        ++m_LoopDepth;
        if (node->Body) { node->Body->Accept(this); }
        --m_LoopDepth;
        return TYPE_BOOL;
    }

    DataType VisitBreak(BreakNode* node) override {
        if (m_LoopDepth <= 0) {
            Surab::LogError("Line {}: 'break' outside loop context", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }
        return TYPE_BOOL;
    }

    DataType VisitContinue(ContinueNode* node) override {
        if (m_LoopDepth <= 0) {
            Surab::LogError("Line {}: 'continue' outside loop context", node->LineNo, node->ToString());
            return TYPE_ERROR;
        }
        return TYPE_BOOL;
    }

    void AddSymbol(const std::string& name, DataType type, bool initialized) {
        m_Symboltable[name] = { name, type, initialized };
    }
private:
    std::unordered_map<std::string, SymbolInfo> m_Symboltable;
    int m_LoopDepth{ 0 };
};

DataType LiteralNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitLiteral(this); }
DataType VariableNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitVariable(this); }
DataType BinaryExprNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitBinaryExpr(this); }
DataType IfNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitIf(this); }
DataType WhileNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitWhile(this); }
DataType BreakNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitBreak(this); }
DataType ContinueNode::Accept(ISemanticVisitor* visitor) { return visitor->VisitContinue(this); }

int main() {
    Header("Semantic Checker");

    SemanticAnalyzer analyzer;
    analyzer.AddSymbol("x", TYPE_INT, true);
    analyzer.AddSymbol("y", TYPE_FLOAT, true);
    analyzer.AddSymbol("flag", TYPE_BOOL, true);

    auto expr1 = new BinaryExprNode("+", new VariableNode("x", LINE), new VariableNode("y", LINE), LINE);
    DataType expr1Type = expr1->Accept(&analyzer);
    Surab::Log("expr1: {} -> type = {}", expr1->ToString(), ToString(expr1Type));

    auto expr2 = new BinaryExprNode("==", new VariableNode("flag", LINE), new LiteralNode(TYPE_BOOL, "true", LINE), LINE);
    DataType expr2Type = expr2->Accept(&analyzer);
    Surab::Log("expr2: {} -> type = {}", expr2->ToString(), ToString(expr2Type));

    auto whileNode = new WhileNode(new VariableNode("flag", LINE), new BreakNode(LINE), LINE);
    DataType whileNodeType = whileNode->Accept(&analyzer);
    Surab::Log("whileExpr: {} -> result = {}", whileNode->ToString(), ToString(whileNodeType));

    auto ifNode = new IfNode(expr2, new ContinueNode(LINE), new BreakNode(LINE), LINE);
    DataType ifNodeType = ifNode->Accept(&analyzer);
    Surab::Log("ifExpr: {} -> result = {}", ifNode->ToString(), ToString(ifNodeType));

    Footer();
    return 0;
}
