#include <iostream>

#include "graphics/graphics.h"
#include "Utility.h"
#include "Maths.h"
#include "Graphics.h"

using namespace CG::Maths;

void drawCircleMidPoint(const Vec2f& center, int r) {
    CG::Graphics::InitGraphics("MidPoint Circle");
    int x = 0, y = r;
    int p = 1 - r;
    while (x <= y) {
        putpixel(center.x + x, center.y + y, WHITE);
        putpixel(center.x - x, center.y + y, WHITE);
        putpixel(center.x + x, center.y - y, WHITE);
        putpixel(center.x - x, center.y - y, WHITE);
        putpixel(center.x + y, center.y + x, WHITE);
        putpixel(center.x - y, center.y + x, WHITE);
        putpixel(center.x + y, center.y - x, WHITE);
        putpixel(center.x - y, center.y - x, WHITE);
        if (p < 0) {
            p += 2 * x + 3;
        }
        else {
            p += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    CG::Graphics::ShutdownGraphics();
}

int main() {
    Vec2f center;
    std::cout << "Enter the center (x, y) of the circle: ";
    std::cin >> center;
    int r;
    std::cout << "Enter the radius of the circle: ";
    std::cin >> r;

    drawCircleMidPoint(center, r);

    CG::Utility::footer();
}