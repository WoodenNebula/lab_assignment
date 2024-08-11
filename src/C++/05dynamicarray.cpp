#include <iostream>

int Sum(int* arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum;
}

int main() {
    int size = 0;
    std::cout << "Enter no. count: ";
    std::cin >> size;

    std::cout << "\nEnter numbers:\n";
    int* arr = new int[size];

    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    std::cout << "Array is:\n";
    for (int i = 0; i < size - 1; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << arr[size - 1] << std::endl;

    std::cout << "Sum of array is " << Sum(arr, size) << std::endl;

    delete[] arr;
}