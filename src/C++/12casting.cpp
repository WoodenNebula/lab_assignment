#include <iostream>

// class Celsius;

class Fahrenheit {
public:
    Fahrenheit() {}
    Fahrenheit(float temp) :m_temp(temp) {}

    friend std::ostream& operator<<(std::ostream& out, const Fahrenheit& f) {
        out << f.m_temp << "F\n";
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Fahrenheit& f) {
        std::cout << "Enter temp in Fahrenheit: ";
        in >> f.m_temp;
        return in;
    }

    friend class Celsius;

    // operator Celsius() {
        // std::cout << "Conversion from F to C\n";
        // return Celsius((m_temp - 32) * 5 / 9);
    // }
private:
    float m_temp;
};



class Celsius {
public:
    Celsius() {}
    Celsius(float temp) :m_temp(temp) {}
    Celsius(const Fahrenheit& from) {
        m_temp = ((from.m_temp - 32) * 5 / 9);
    }

    friend std::ostream& operator<<(std::ostream& out, const Celsius& c) {
        out << c.m_temp << "C\n";
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Celsius& c) {
        std::cout << "Enter temp in Celsius: ";
        in >> c.m_temp;
        return in;
    }


    // operator Fahrenheit() const {
    //     std::cout << "Conversion from C to F\n";
    //     return Fahrenheit(m_temp * 9 / 5 + 32);
    // }

private:
    float m_temp;
};


int main() {
    Fahrenheit f;
    std::cin >> f;
    std::cout << f;
    // std::cout << (Fahrenheit)c << std::endl;

    std::cout << Celsius(f) << std::endl;

    // Fahrenheit f = (Fahrenheit)c;
    // std::cout << f;
    // std::cout << (Celsius)f << std::endl;
}