#include "src/commons.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <source_location>

enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_ARRAY,
    TYPE_ERROR
};

struct SymbolInfo {
    std::string name;
    DataType type = TYPE_INT;
    DataType elementType = TYPE_INT;
    int size = 0;
    std::vector<int> dimensions;
};

std::string ToString(DataType type) {
    switch (type) {
    case TYPE_INT: return "int";
    case TYPE_FLOAT: return "float";
    case TYPE_BOOL: return "bool";
    case TYPE_ARRAY: return "array";
    case TYPE_ERROR: return "error";
    default: return "unknown";
    }
}

bool IsArithmeticType(DataType type) {
    return type == TYPE_INT || type == TYPE_FLOAT;
}

bool IsLogicalType(DataType type) {
    return type == TYPE_BOOL || type == TYPE_INT;
}

DataType EvaluateArithmeticType(DataType left, DataType right, const std::string& op, int lineNo) {
    if (left == TYPE_ERROR || right == TYPE_ERROR) {
        return TYPE_ERROR;
    }

    // Coercion rules:
    // INT + INT -> INT
    // INT + FLOAT -> FLOAT (implicit promotion)
    // FLOAT + FLOAT -> FLOAT
    // anything else like INT + BOOL is invalid and logs an explicit error
    if (left == TYPE_INT && right == TYPE_INT) {
        return TYPE_INT;
    }

    if ((left == TYPE_INT && right == TYPE_FLOAT) || (left == TYPE_FLOAT && right == TYPE_INT) ||
        (left == TYPE_FLOAT && right == TYPE_FLOAT)) {
        return TYPE_FLOAT;
    }

    Surab::LogError("Line {}: Type Error - Incompatible operands: {} {} {}",
        lineNo, ToString(left), op, ToString(right));
    return TYPE_ERROR;
}

DataType EvaluateAssignmentType(DataType targetType, DataType sourceType, int lineNo) {
    if (targetType == TYPE_ERROR || sourceType == TYPE_ERROR) {
        return TYPE_ERROR;
    }

    if (targetType == sourceType) {
        return targetType;
    }

    if (targetType == TYPE_FLOAT && sourceType == TYPE_INT) {
        return TYPE_FLOAT;
    }

    Surab::LogError("Line {}: Assignment type mismatch: cannot assign {} to {}.", lineNo, ToString(sourceType), ToString(targetType));
    return TYPE_ERROR;
}

DataType EvaluateLogicalType(DataType left, DataType right, const std::string& op, int lineNo) {
    if (left == TYPE_ERROR || right == TYPE_ERROR) {
        return TYPE_ERROR;
    }

    if (op == "&&" || op == "||") {
        if (IsLogicalType(left) && IsLogicalType(right)) {
            return TYPE_BOOL;
        }
        Surab::LogError("Line {}: Logical type error: {} {} {} is invalid.", lineNo, ToString(left), op, ToString(right));
        return TYPE_ERROR;
    }

    if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") {
        if ((IsArithmeticType(left) && IsArithmeticType(right)) || (left == TYPE_BOOL && right == TYPE_BOOL)) {
            return TYPE_BOOL;
        }
        Surab::LogError("Line {}: Comparison type error: {} {} {} is invalid.", lineNo, ToString(left), op, ToString(right));
        return TYPE_ERROR;
    }

    Surab::LogError("Line {}: Unsupported logical operator '{}'.", lineNo, op);
    return TYPE_ERROR;
}

bool IsValidArrayIndex(DataType indexType, int lineNo) {
    if (indexType != TYPE_INT) {
        Surab::LogError("Line {}: Array index must be of type int.", lineNo);
        return false;
    }
    return true;
}

DataType EvaluateArrayReference(const SymbolInfo& array, DataType indexType, int lineNo) {
    if (array.type != TYPE_ARRAY) {
        Surab::LogError("Line {}: {} is not an array.", lineNo, array.name);
        return TYPE_ERROR;
    }

    if (!IsValidArrayIndex(indexType, lineNo)) {
        Surab::LogError("Line {}: {} is not a valid array index type", lineNo, ToString(indexType));
        return TYPE_ERROR;
    }

    if (!array.dimensions.empty() && array.dimensions[0] <= 0) {
        Surab::LogError("Line {}: Array '{}' has invalid dimensions.", lineNo, array.name);
        return TYPE_ERROR;
    }

    return array.elementType;
}

void PrintResult(const std::string& label, DataType result) {
    Surab::Log("{} -> {}", label, ToString(result));
}

int main() {
    Header("Type Checker");

    const DataType i = TYPE_INT;
    const DataType f = TYPE_FLOAT;
    const DataType b = TYPE_BOOL;

#define LINE static_cast<int>(std::source_location::current().line())

    Surab::Log("{:=^20}", "Arithmetic expressions");
    PrintResult("INT + INT", EvaluateArithmeticType(i, i, "+", LINE));
    PrintResult("INT + FLOAT", EvaluateArithmeticType(i, f, "+", LINE));
    PrintResult("FLOAT / FLOAT", EvaluateArithmeticType(f, f, "/", LINE));
    PrintResult("INT + BOOL (failure)", EvaluateArithmeticType(i, b, "+", LINE));

    Surab::Log("\n{:=^20}", "Assignments");
    PrintResult("INT = INT", EvaluateAssignmentType(i, i, LINE));
    PrintResult("FLOAT = INT", EvaluateAssignmentType(f, i, LINE));
    PrintResult("INT = FLOAT (failure)", EvaluateAssignmentType(i, f, LINE));

    Surab::Log("\n{:=^20}", "Logical checks");
    PrintResult("BOOL && BOOL", EvaluateLogicalType(b, b, "&&", LINE));
    PrintResult("BOOL || BOOL", EvaluateLogicalType(b, b, "||", LINE));
    PrintResult("INT < INT", EvaluateLogicalType(i, i, "<", LINE));
    PrintResult("INT == INT", EvaluateLogicalType(i, i, "==", LINE));
    PrintResult("BOOL && FLOAT (failure)", EvaluateLogicalType(b, f, "&&", LINE));

    Surab::Log("\n{:=^20}", "Array references");
    SymbolInfo arr{ "arr", TYPE_ARRAY, TYPE_INT, 16, { 10, 20 } };
    PrintResult("array[int]", EvaluateArrayReference(arr, i, LINE));
    PrintResult("array[bool] (failure)", EvaluateArrayReference(arr, b, LINE));

    Footer();
    return 0;
}
