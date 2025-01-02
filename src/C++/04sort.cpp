#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
bool compare(T x, T y) {
    return x < y;
}

void Sort(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end(), compare<int>);
}

void Sort(std::vector<char>& arr) {
    std::sort(arr.begin(), arr.end(), compare<char>);
}

void Sort(std::vector<float>& arr) {
    std::sort(arr.begin(), arr.end(), compare<float>);
}


int main() {
    std::vector<int> i_arr = { 4, 2, 3, 0, 12, 23, 2, 1, 1 };
    std::vector<char> c_arr = { 'a', 'h', 'w', 't', 'e', ',', '!' };
    std::vector<float> f_arr = { 9.0f, 2.0f, 0.2f, 0.24f, 1.0f };

    Sort(i_arr);
    Sort(c_arr);
    Sort(f_arr);

    for (auto x : i_arr) {
        std::cout << x << ", ";
    }

    std::cout << "\n-------" << std::endl;

    for (auto x : c_arr) {
        std::cout << x << ", ";
    }

    std::cout << "\n-------" << std::endl;

    for (auto x : f_arr) {
        std::cout << x << ", ";
    }
    std::cout <<  std::endl;
}