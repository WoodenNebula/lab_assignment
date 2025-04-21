#include <iostream>

#include "graphics/graphics.h"
#include "Utility.h"
#include "Maths.h"
#include "Graphics.h"

using namespace CG::Maths;

void draw(const Vec2f& center, const Vec2f& point) {
    delay(10);
    putpixel(center.x + point.x, center.y + point.y, WHITE);
    putpixel(center.x - point.x, center.y + point.y, WHITE);
    putpixel(center.x + point.x, center.y - point.y, WHITE);
    putpixel(center.x - point.x, center.y - point.y, WHITE);
}

void drawEllipse(const Vec2f& center, const Vec2f& radii) {
    CG::Graphics::InitGraphics("Midpoint Ellipse");
    Vec2f p = { 0, radii.y };
    float p1 = (radii.y * radii.y) - (radii.x * radii.x * radii.y) + (radii.x * radii.x) / 4;
    while ((2.0f * radii.y * radii.y * p.x) < (2.0f * radii.x * radii.x * p.y)) {
        draw(center, p);
        p.x++;
        if (p1 < 0) {
            p1 += (2.0f * radii.y * radii.y * p.x) + (radii.y * radii.y);
        }
        else {
            p.y--;
            p1 += (2.0f * radii.y * radii.y * p.x) - (2.0f * radii.x * radii.x * p.y) + (radii.y * radii.y);
        }
    }
    float p2 = (radii.y * radii.y) * (p.x + 0.5f) * (p.x + 0.5f) + (radii.x * radii.x * (p.y - 1.0f) * (p.y - 1.0f) - (radii.x * radii.x) * (radii.y * radii.y));
    while (p.y >= 0.0f) {
        draw(center, p);
        p.y--;
        if (p2 > 0.0f) {
            p2 += (radii.x * radii.x) - (2.0f * radii.x * radii.x * p.y);
        }
        else {
            p.x++;
            p2 += (2.0f * radii.y * radii.y * p.x) - (2.0f * radii.x * radii.x * p.y) + (radii.x * radii.x);
        }
    }
    CG::Graphics::ShutdownGraphics();
}

int main() {
    Vec2f center;
    std::cout << "Enter the center (x, y) of the ellipse: ";
    std::cin >> center;
    Vec2f radii;
    std::cout << "Enter the radii of the ellipse: ";
    std::cin >> radii;

    drawEllipse(center, radii);

    CG::Utility::footer();
}