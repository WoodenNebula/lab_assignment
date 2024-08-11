#include <iostream>

struct Distance {
public:
    Distance() {}
    Distance(float meter) : m_m(meter), m_cm(m_m * 100.0f) {}

    Distance Add(const Distance& other) const {
        return Distance(m_m + other.m_m);
    }

    friend std::ostream& operator<<(std::ostream& out, const Distance& d);
    friend std::istream& operator>>(std::istream& in, Distance& d);

private:
    float m_m;
    float m_cm;
};

std::istream& operator>>(std::istream& in, Distance& d) {
    std::cout << "Enter distance in meter: ";
    in >> d.m_m;
    d.m_cm = d.m_m * 100.0f;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Distance& d) {
    out << d.m_m << "m";
    return out;
}


int main() {
    Distance d1(2.0f), d2;
    std::cin >> d1 >> d2;
    std::cout << "Sum of d1(" << d1 << ") and d2(" << d2 << ") is " << d1.Add(d2) << std::endl;
}