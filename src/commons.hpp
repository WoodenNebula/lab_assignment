#pragma once
#include <ios>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <source_location>

#define RED "\033[91m"
#define CYAN "\033[96m"
#define GREY "\033[90m"
#define GREEN "\033[92m"
#define RESET "\033[0m"

#if (LOG_LEVEL == 1)
#define LOG(x) printf("\n[LOG] : %s\n", x)
#else
#define LOG(x)
#endif

#define LINE static_cast<int>(std::source_location::current().line())

static void Header(std::string_view Title) {
    std::cout << "=== " << std::uppercase << Title << " ===" << std::endl;
}

static void Footer() {
    std::cout << "\n====================\n"
        << "Surab Parajuli\n"
        << "Section: A, 6th-Sem\n"
        << "Roll: 34, Symbol No.: 80010139\n"
        << "====================\n";
#ifndef __linux
    //  syst("read -p \"Press any key to continue...\"");
    system("pause");
#endif
}

static void abortOnError(std::string_view msg) {
    std::cout << RED << msg << RESET;
    Footer();
    exit(EXIT_FAILURE);
}

namespace Surab
{

std::string C2S(char c) { return std::string(1, c); }

std::string Trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    return s;
}

using Mat = std::vector<std::vector<double>>;

template <std::ranges::sized_range ContainerType>
std::string ToString(const ContainerType& Container, const std::string& Delimiter = ", ", bool UseBrackets = true) {
    if (Container.empty())
        return UseBrackets ? "[ ]" : "";

    std::string containerStr = UseBrackets ? "[ " : "";

    for (const auto x : Container) {
        if constexpr (std::convertible_to<decltype(x), std::string>)
            containerStr += x + Delimiter;
        else
            containerStr += std::to_string(x) + Delimiter;
    }

    // Remove the last delimiter
    containerStr.erase(containerStr.size() - Delimiter.size());
    if (UseBrackets)
        containerStr += " ]";

    return containerStr;
}

}  // namespace Surab

static std::istream& operator>>(std::istream& in, Surab::Mat& matrix) {
    for (size_t row = 0; row < matrix.size(); row++) {
        std::cout << "  row[" << row + 1 << "] <- ";
        for (size_t col = 0; col < matrix[row].size(); col++) {
            in >> matrix[row][col];
        }
    }
    std::cout << "\n";
    return in;
}

static std::ostream& operator<<(std::ostream& out, const Surab::Mat& matrix) {
    for (size_t row = 0; row < matrix.size(); row++) {
        out << "\t" << Surab::ToString(matrix[row]) << "\n";
    }
    return out;
}

namespace Surab
{
template <typename... Args>
static void LogError(std::format_string<Args...> fmt, Args&&... args) {
    std::cerr << RED << std::format(fmt, std::forward<Args>(args)...) << RESET << std::endl;
}

template <typename... Args>
static void LogSuccess(std::format_string<Args...> msg, Args&&... args) {
    std::cout << GREEN << std::format(msg, std::forward<Args>(args)...) << RESET << std::endl;
}

template <typename... Args>
static void Log(std::format_string<Args...> msg, Args&&... args) {
    std::cout << std::format(msg, std::forward<Args>(args)...) << std::endl;
}
}  // namespace Surab
