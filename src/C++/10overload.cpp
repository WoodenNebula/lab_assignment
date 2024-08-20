#include <iostream>

class Distance {
public:
    Distance() {}
    Distance(int f, int i) :m_feet(f + i / 12), m_inch(i % 12) {}
    Distance(Distance& d) :Distance(d.m_feet, d.m_inch) {}

    friend std::ostream& operator<<  (std::ostream& out, const Distance& d) {
        out << d.m_feet << "\'" << d.m_inch << "\"";
        return out;
    }

    // friend std::istream& operator>> (std::istream& in, Distance& d) {
        // std::cout << "Enter Feet and inches: ";
        // int feet, inch;
        // in >> feet >> inch;
// 
        // d.m_inch = inch % 12;
        // d.m_feet = feet + inch / 12;
        // return in;
    // }

    Distance operator+(const Distance& other) {
        return Sum(other);
    }

    Distance Sum(const Distance& other) const {
        return { m_feet + other.m_feet, m_inch + other.m_inch };
    }

private:
    int m_feet{};
    int m_inch{};
};


class String {
public:
    String(const std::string& str) : m_data(str) {}

    String operator+(const String& other) const {
        return m_data + " " + other.m_data;
    }

    friend std::ostream& operator<<(std::ostream& out, const String& s) {
        out << s.m_data;
        return out;
    }

private:
    std::string m_data;
};


int main() {
    String s1("Hello"), s2("World!");
    std::cout << "Adding s1(" << s1 << ")" << " and s2(" << s2 << ") = " << s1 + s2 << std::endl;

    Distance d1(2, 4), d2(3, 17);
    std::cout << "Adding d1(" << d1 << ") and d2(" << d2 << ") = " << d1 + d2 << std::endl;
}