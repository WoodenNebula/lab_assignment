#include <iostream>
#include <string>

class Distance {
public:
    Distance() {}
    Distance(int f, int i) :m_feet(f + i / 12), m_inch(i % 12) {}
    Distance(Distance& d) :Distance(d.m_feet, d.m_inch) {}

    void Get() const {
        std::cout << m_feet << "\'" << m_inch << "\"";
    }

    void Set() {
        std::cout << "Enter Feet and inches: ";
        int feet, inch;
        std::cin >> feet >> inch;

        m_inch = inch % 12;
        m_feet = feet + inch / 12;
    }

    Distance Sum(const Distance& other) const {
        return { m_feet + other.m_feet, m_inch + other.m_inch };
    }

private:
    int m_feet{};
    int m_inch{};
};

int main() {
    Distance d1, copyTarget;

    d1.Set();
    copyTarget.Set();

    Distance d2(copyTarget);

    std::cout << "Adding d1 = ";
    d1.Get();
    std::cout << " and d2 = ";
    d2.Get();

    std::cout << "\nSum = ";
    d1.Sum(d2)
        .Get();
    std::cout << std::endl;
}