#include <iostream>
#include <vector>

using BM = std::vector<std::vector<bool>>;

std::ostream& operator<<(std::ostream& os, const BM& matrix) {
  for (const auto& row : matrix) {
    os << "| ";
    for (const auto& num : row) {
      os << num << " ";
    }
    os << "|" << std::endl;
  }
  return os;
}

BM join(const BM& A, const BM& B) {
  BM result;
  for (size_t i = 0; i < A.size(); i++) {
    std::vector<bool> row;
    for (size_t j = 0; j < A[i].size(); j++) {
      row.push_back(A[i][j] || B[i][j]);
    }
    result.push_back(row);
  }
  return result;
}

BM meet(const BM& A, const BM& B) {
  BM result;
  for (size_t i = 0; i < A.size(); i++) {
    std::vector<bool> row;
    for (size_t j = 0; j < A[i].size(); j++) {
      row.push_back(A[i][j] && B[i][j]);
    }
    result.push_back(row);
  }
  return result;
}

BM product(const BM& A, const BM& B) {
  BM result;
  for (size_t i = 0; i < A.size(); i++) {
    std::vector<bool> row;
    for (size_t j = 0; j < B[i].size(); j++) {
      bool value = false;
      for (size_t k = 0; k < A[i].size(); k++) {
        value = value || (A[i][k] && B[k][j]);
      }
      row.push_back(value);
    }
    result.push_back(row);
  }
  return result;
}

int main() {
  BM A = {
    { 1, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 1 } };
  BM B = {
    { 0, 1, 1 },
    { 0, 1, 0 },
    { 1, 1, 0 } };

  // std::cout << "Matrix A :\n" << A << std::endl;
  // std::cout << "Matrix B :\n" << B << std::endl;

  std::cout << "Join :\n" << join(A, B) << std::endl;
  std::cout << "Meet :\n" << meet(A, B) << std::endl;
  std::cout << "Product :\n" << product(A, B) << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}