#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
void Sort(std::vector<T>& arr) {
    std::sort(arr.begin(), arr.end());
}

template<typename T>
void Print(const std::vector<T>& arr) {
    for (auto x : arr) {
        std::cout << x << ", ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> i_arr = { 0, 2, 4, 5, -1, 5, 78, 1, -5, 0 };
    std::vector<char> c_arr = { '1', 'a', 'd', 'g', 'w', '%', '_' };
    std::vector<float> f_arr = { 1.0f, 9.0f, 2.019f, 0.067f, 0.69f };

    std::cout << "Before sorting:\n";

    std::cout << "Int arr = ";
    Print(i_arr);

    std::cout << "Char arr = ";
    Print(c_arr);

    std::cout << "Float arr = ";
    Print(f_arr);

    std::cout << "After Sorting:\n";

    Sort(i_arr);
    Sort(c_arr);
    Sort(f_arr);

    std::cout << "Int arr = ";
    Print(i_arr);

    std::cout << "Char arr = ";
    Print(c_arr);

    std::cout << "Float arr = ";
    Print(f_arr);
}