#pragma once
#include <iostream>
#include <limits>

namespace CG::Math
{
template <typename T>
struct Vec2 {
    T x, y;
    Vec2(T x = 0, T y = 0) : x(x), y(y) {}
    Vec2<T> operator+(const Vec2<T>& other) const { return { x + other.x, y + other.y }; }
    Vec2<T> operator-(const Vec2<T>& other) const { return { x - other.x, y - other.y }; }
    Vec2<T> operator*(float scalar) const { return { x * scalar, y * scalar }; }
    friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vec2<T>& vec) {
        is >> vec.x >> vec.y;
        return is;
    }
};
template <typename T>
struct Vec3 {
    // allow to access as x, y, z or r, g, b
    union {
        struct { T x, y, z; };
        struct { T r, g, b; };
    };
    Vec3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vec3& vec) {
        is >> vec.x >> vec.y >> vec.z;
        return is;
    }
};
}

namespace CG
{
using Vec2f = CG::Math::Vec2<float>;
using Vec3f = CG::Math::Vec3<float>;

Vec3f getRandomColor() {
    srand(static_cast<unsigned int>(time(0)));
    return { static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX };
};

void flush() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void pause() {
    flush();
    std::cout << "Press any key to continue...";
    std::cin.get();
}
void footer() {
    printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
    pause();
}
}
