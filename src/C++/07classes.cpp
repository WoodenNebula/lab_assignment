#include <iostream>

class Time {
public:
    Time() {}
    Time(int hour, int min, int sec) : m_hour{ hour }, m_minute{ min }, m_second{ sec } {}

    void Set() {
        std::cout << "Enter Hour, Minute and second respectively:\n";
        std::cin >> m_hour >> m_minute >> m_second;
    }

    friend std::ostream& operator<<(std::ostream& out, const Time& t) {
        out << t.m_hour << ":" << t.m_minute << ":" << t.m_second;
        return out;
    }

    friend Time AddByValue(const Time, const Time);
    friend Time AddTime(const Time&, const Time&);
    friend Time AddTime(const Time*, const Time*);

private:
    int m_hour{};
    int m_minute{};
    int m_second{};
};

Time AddByValue(const Time t1, const Time t2) {
    int s = t1.m_second + t2.m_second;

    int min = t1.m_minute + t2.m_minute + s / 60;
    s %= 60;

    int hour = t1.m_hour + t2.m_hour + min / 60;
    min %= 60;

    return Time(hour, min, s);
}

Time AddTime(const Time& t1, const Time& t2) {
    int s = t1.m_second + t2.m_second;

    int min = t1.m_minute + t2.m_minute + s / 60;
    s %= 60;

    int hour = t1.m_hour + t2.m_hour + min / 60;
    min %= 60;

    return Time(hour, min, s);
}

Time AddTime(const Time* t1, const Time* t2) {
    int s = t1->m_second + t2->m_second;

    int min = t1->m_minute + t2->m_minute + s / 60;
    s %= 60;

    int hour = t1->m_hour + t2->m_hour + min / 60;
    min %= 60;

    return Time(hour, min, s);
}

int main() {
    Time t1, t2, t3, t4, t5, t6;
    t1.Set();
    t2.Set();

    t3.Set();
    t4.Set();

    t5.Set();
    t6.Set();

    std::cout << "Adding t1 = " << t1 << " and t2 = " << t2 << std::endl;
    std::cout << "Sum =  " << AddByValue(t1, t2) << std::endl;

    std::cout << "Adding t3 = " << t3 << " and t4 = " << t4 << std::endl;
    std::cout << "Sum =  " << AddTime(t3, t4) << std::endl;

    std::cout << "Adding t5 = " << t5 << " and t6 = " << t6 << std::endl;
    std::cout << "Sum =  " << AddTime(&t5, &t6) << std::endl;
}