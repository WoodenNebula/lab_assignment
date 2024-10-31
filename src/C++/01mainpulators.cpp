#include <iostream>
#include <iomanip>

int main() {
    std::string num[5] = { "S.No.", "1.", "2.", "3.", "4." };
    std::string cities[5] = { "City", "KATHMANDU", "Pokhara", "Butwal", "Dharan" };
    std::string districts[5] = { "District", "Kathmandu", "Kaski", "Rupendehi", "Sunsari" };
    std::string provinces[5] = { "Province", "Bagmati", "Gandaki", "Lumbini", "Prov 1" };
    std::string populations[5] = { "Population", "500000", "50000", "100000", "80000" };

    for (int i = 0; i < 5; i++) {
        std::cout << std::setw(6) << std::left << num[i]
         << std::setw(14) << std::left << cities[i]
         << std::setw(14) << std::left << districts[i]
         << std::setw(14) << std::left << provinces[i]
         << std::setw(11) << std::right << populations[i] << std::endl;
    }
}