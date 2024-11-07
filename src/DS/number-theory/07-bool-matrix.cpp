#include <iostream>
#include <vector>

using BoolMatrix = std::vector<std::vector<bool>>;

std::ostream& operator<<(std::ostream& os, const BoolMatrix& matrix) {
  for (const auto& row : matrix) {
    os << "| ";
    for (const auto& num : row) {
      os << num << " ";
    }
    os << "|" << std::endl;
  }
  return os;
}

BoolMatrix join(const BoolMatrix& matrixA, const BoolMatrix& matrixB) {
  BoolMatrix result;
  for (int i = 0; i < matrixA.size(); i++) {
    std::vector<bool> row;
    for (int j = 0; j < matrixA[i].size(); j++) {
      row.push_back(matrixA[i][j] || matrixB[i][j]);
    }
    result.push_back(row);
  }
  return result;
}

BoolMatrix meet(const BoolMatrix& matrixA, const BoolMatrix& matrixB) {
  BoolMatrix result;
  for (int i = 0; i < matrixA.size(); i++) {
    std::vector<bool> row;
    for (int j = 0; j < matrixA[i].size(); j++) {
      row.push_back(matrixA[i][j] && matrixB[i][j]);
    }
    result.push_back(row);
  }
  return result;
}

BoolMatrix product(const BoolMatrix& matrixA, const BoolMatrix& matrixB) {
  BoolMatrix result;
  for (int i = 0; i < matrixA.size(); i++) {
    std::vector<bool> row;
    for (int j = 0; j < matrixB[i].size(); j++) {
      bool value = false;
      for (int k = 0; k < matrixA[i].size(); k++) {
        value = value || (matrixA[i][k] && matrixB[k][j]);
      }
      row.push_back(value);
    }
    result.push_back(row);
  }
  return result;
}

int main() {
  BoolMatrix matrixA = {
    { 1, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 1 } };
  BoolMatrix matrixB = {
    { 0, 1, 1 },
    { 0, 1, 0 },
    { 1, 1, 0 } };

  std::cout << "Matrix A :\n" << matrixA << std::endl;
  std::cout << "Matrix B :\n" << matrixB << std::endl;

  std::cout << "Join :\n" << join(matrixA, matrixB) << std::endl;
  std::cout << "Meet :\n" << meet(matrixA, matrixB) << std::endl;
  std::cout << "Product :\n" << product(matrixA, matrixB) << std::endl;
}