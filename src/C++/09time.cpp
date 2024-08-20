#include <iostream>

class Time {
public:
    Time(int h = 1, int m = 1, int s = 1) : m_hour(h), m_min(m), m_sec(s) {}

    Time& operator++() {
        std::cout << "Pre increment called!" << std::endl;
        m_min += ((++m_sec) / 60);
        m_sec %= 60;

        m_hour += (m_min / 60);
        m_min %= 60;

        return *this;
    }

    Time operator++(int) {
        std::cout << "Post Increment called!" << std::endl;
        Time c(*this);
        ++(*this);

        return c;
    }

    friend std::ostream& operator <<(std::ostream& out, const Time& t) {
        out << t.m_hour << ":" << t.m_min << ":" << t.m_sec << "s" << std::endl;
        return out;
    }

    Time operator+(const Time& other) {
        return { m_hour + other.m_hour, m_min + other.m_min, m_sec + other.m_sec };
    }

private:
    int m_hour;
    int m_min;
    int m_sec;
};

int main() {
    Time t(1, 59, 59);
    std::cout << "t1 = " << t;
    Time t1 = ++t;
    std::cout << "\nt1 = ++t \n";
    std::cout << "Pre incrementing t: " << t;
    std::cout << "Pre incrementing assignment t1: " << t1;

    std::cout << "\nt1 = t++ \n";
    t1 = t++;
    std::cout << "Post incrementing t: " << t;
    std::cout << "Post incrementing assigment t1: " << t1;
}