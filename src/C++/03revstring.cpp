#include <iostream>
#include <string>

std::string reverse(const std::string& target) {
    std::string rev;
    for (auto itr = target.rbegin(); itr != target.rend(); itr++) {
        rev.push_back(*itr);
    }
    return rev;
}

int main() {
    std::string in;
    std::cout << "Enter string:\n";
    std::cin >> in;
    std::cout << in << std::endl;
    std::cout << reverse(in) << std::endl;
}