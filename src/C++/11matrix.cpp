#include <iostream>
#include <vector>
#include <array>

class Matrix {
public:
    Matrix(std::vector<int> mat = std::vector<int>(4, 0)) :m_mat(mat) {}

    void Read() {
        std::cout << "Enter 2x2 Matrix:\n";
        for (int& i : m_mat) {
            std::cin >> i;
        }
    }

    void Show() const {
        int i = 1;
        for (int x : m_mat) {
            std::cout << x;
            if (i % 2 == 0) {
                std::cout << '\n';
            }
            else {
                std::cout << '\t';
            }
            i++;
        }
        std::cout << std::endl;
    }


    Matrix operator+(const Matrix& other) {
        return { std::vector<int>{
            m_mat[0] + other.m_mat[0], m_mat[1] + other.m_mat[1],
                m_mat[2] + other.m_mat[2], m_mat[3] + other.m_mat[3]
        } };
    }


    Matrix operator*(const Matrix& other) {
        std::vector<int> prod(4, 0);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                int temp_prod = 0;
                for (int column = 0, row = 0; column < m_column || row < m_row; column++, row++) {
                    int idx = 2 * i + j;
                    temp_prod = m_mat[2 * i + column] * other.m_mat[2 * row + j];
                    prod[idx] += temp_prod;
                }
            }
        }
        return { prod };
    }

private:
    int m_row = 2;
    int m_column = 2;
    std::vector<int> m_mat;
};


int main() {
    Matrix m;
    m.Read();
    // m.Show();

    Matrix m1;
    m1.Read();
    // m1.Show();

    std::cout << "Sum is:\n";
    (m1 + m).Show();

    std::cout << "Prod is:\n";
    (m * m1).Show();
}