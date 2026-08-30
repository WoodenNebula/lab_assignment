#include "src/commons.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Symbol {
    std::string name;
    std::string type;
    int address = 0;
    int size = 0;
};

int findSymbol(const std::vector<Symbol>& table, const std::string& name) {
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].name == name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void displaySymbols(const std::vector<Symbol>& table) {
    if (table.empty()) {
        Surab::Log("No symbols available.");
        return;
    }

    std::cout << "\n";
    std::cout << std::left << std::setw(8) << "Index" << " | "
        << std::setw(18) << "Name" << " | "
        << std::setw(10) << "Type" << " | "
        << std::setw(10) << "Address" << " | "
        << "Size\n";
    std::cout << std::string(72, '-') << "\n";

    for (size_t i = 0; i < table.size(); ++i) {
        const auto& sym = table[i];
        std::cout << std::left << std::setw(8) << i << " | "
            << std::setw(18) << sym.name << " | "
            << std::setw(10) << sym.type << " | "
            << std::setw(10) << sym.address << " | "
            << sym.size << "\n";
    }
}

void printMenu() {
    std::cout << "\n=== Symbol Table Menu ===\n";
    std::cout << "1. Insert\n";
    std::cout << "2. Display\n";
    std::cout << "3. Delete\n";
    std::cout << "4. Search\n";
    std::cout << "5. Modify\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    Header("Symbol Table Implementation");

    std::vector<Symbol> symbolTable;

    while (true) {
        printMenu();

        int choice = 0;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 6) {
            break;
        }

        switch (choice) {
        case 1:
        {
            Symbol symbol;
            std::cout << "Enter symbol name: ";
            std::getline(std::cin, symbol.name);

            if (symbol.name.empty()) {
                Surab::LogError("Invalid symbol name.");
                break;
            }

            if (findSymbol(symbolTable, symbol.name) != -1) {
                Surab::LogError("Duplicate symbol");
                break;
            }

            std::cout << "Enter symbol type: ";
            std::getline(std::cin, symbol.type);
            std::cout << "Enter address: ";
            std::cin >> symbol.address;
            std::cout << "Enter size: ";
            std::cin >> symbol.size;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            symbolTable.push_back(symbol);
            Surab::LogSuccess("Symbol inserted successfully.");
            break;
        }

        case 2:
            displaySymbols(symbolTable);
            break;

        case 3:
        {
            std::string target;
            std::cout << "Enter symbol name to delete: ";
            std::getline(std::cin, target);

            const int index = findSymbol(symbolTable, target);
            if (index == -1) {
                Surab::LogError("Label not found");
                break;
            }

            symbolTable.erase(symbolTable.begin() + index);
            Surab::LogSuccess("Symbol deleted successfully.");
            break;
        }

        case 4:
        {
            std::string target;
            std::cout << "Enter symbol name to search: ";
            std::getline(std::cin, target);

            const int index = findSymbol(symbolTable, target);
            if (index == -1) {
                Surab::LogError("Symbol not found");
                break;
            }

            const Symbol& sym = symbolTable[index];
            std::cout << "\nSymbol found at index " << index << "\n";
            std::cout << "Name: " << sym.name << "\n";
            std::cout << "Type: " << sym.type << "\n";
            std::cout << "Address: " << sym.address << "\n";
            std::cout << "Size: " << sym.size << "\n";
            break;
        }

        case 5:
        {
            std::string target;
            std::cout << "Enter symbol name to modify: ";
            std::getline(std::cin, target);

            const int index = findSymbol(symbolTable, target);
            if (index == -1) {
                Surab::LogError("Symbol not found");
                break;
            }

            std::cout << "Enter new type (leave empty to keep current): ";
            std::string newType;
            std::getline(std::cin, newType);
            if (!newType.empty()) {
                symbolTable[index].type = newType;
            }

            std::cout << "Enter new address (enter 0 to keep current): ";
            int newAddress = 0;
            std::cin >> newAddress;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (newAddress != 0) {
                symbolTable[index].address = newAddress;
            }

            Surab::LogSuccess("Symbol modified successfully.");
            break;
        }

        default:
            Surab::LogError("Invalid option. Please enter a valid choice.");
            break;
        }
    }

    Footer();
    return 0;
}
