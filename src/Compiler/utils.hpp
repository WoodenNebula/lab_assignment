#include "src/commons.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace Surab
{
namespace Compiler
{
std::string Trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    return s;
}

using ProdList = std::vector<std::string>;
ProdList SplitProductions(const std::string& rhs) {
    ProdList parts;
    std::stringstream ss(rhs);
    std::string item;
    while (std::getline(ss, item, '|')) {
        parts.push_back(Trim(item));
    }
    return parts;
}

using Grammar = std::unordered_map<std::string, ProdList>;
Grammar ParseGrammarFromString(std::stringstream& Input) {
    Grammar G;
    std::string lineBuffer;
    while (std::getline(Input, lineBuffer)) {
        // Find the non-terminal
        auto pos = lineBuffer.find("->");
        if (pos == std::string::npos)
            continue;

        std::string LHS = Trim(lineBuffer.substr(0, pos));
        std::string RHS = Trim(lineBuffer.substr(pos + 2));

        auto parts = SplitProductions(RHS);
        for (auto& p : parts) G[LHS].push_back(p);
    }
    return G;
}

void PrintGrammar(const Grammar& G) {
    for (const auto& [A, prods] : G) {
        std::string rhs;
        for (size_t i = 0; i < prods.size(); ++i) {
            rhs += prods[i];
            if (i != prods.size() - 1) rhs += " | ";
        }
        std::println("{} -> {}", A, rhs);
    }
}

} // namespace Compiler
}
