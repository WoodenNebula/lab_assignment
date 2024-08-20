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


    operator Fahrenheit() const {
        std::cout << "Conversion from C to F\n";
        return Fahrenheit(m_temp * 9 / 5 + 32);
    }

private:
    float m_temp;
};


int main() {
    Celsius c;
    std::cin >> c;
    std::cout << c;
    std::cout << (Fahrenheit)c << std::endl;


    // Fahrenheit f = (Fahrenheit)c;
    // std::cout << f;
    // std::cout << (Celsius)f << std::endl;
}