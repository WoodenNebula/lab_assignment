#include <iostream>
#include <GL/glut.h>

#include "graphics/graphics.h"
#include "Window.h"
#include "Polygon.h"
#include "Maths.h"

namespace CG::Maths
{
Polygon translate(const Polygon& poly, const Vec2f& translation) {
    Polygon translatedPoly = poly;
    for (auto& vertex : translatedPoly.vertices) {
        // std::cout << "(" << vertex << "->";
        vertex = vertex + translation;
        // std::cout << vertex << ")" << std::endl;
    }
    return translatedPoly;
}

Polygon rotate(const Polygon& poly, float angle) {
    Polygon rotatedPoly = poly;
    for (auto& vertex : rotatedPoly.vertices) {
        float x = vertex.x;
        float y = vertex.y;
        vertex.x = x * cos(angle) - y * sin(angle);
        vertex.y = x * sin(angle) + y * cos(angle);
    }
    return rotatedPoly;
}
};

using namespace CG::Maths;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    int dt = 750;

    Vec2f center = { 200.0f, 125.0f };
    Vec2f dimensions = { 200.0f, 50.0f };

    CG::Rectangle rectangle(center, dimensions, { .5f, .0f, .4f });
    rectangle.DrawFill();
    glutSwapBuffers();
    delay(dt);

    glClear(GL_COLOR_BUFFER_BIT);
    CG::Polygon tr(translate(rectangle, { -center.x, -center.y }));
    tr.DrawOutline(2.0f);
    glutSwapBuffers();
    delay(dt);

    glClear(GL_COLOR_BUFFER_BIT);
    tr = rotate(tr, 45.0f);
    tr.DrawOutline(2.0f, { 0.0f, 1.0f, 0.0f });
    glutSwapBuffers();
    delay(dt);

    glClear(GL_COLOR_BUFFER_BIT);
    tr = translate(tr, { center.x, center.y });
    tr.color = { 0.0f, 1.0f, 0.0f };
    tr.DrawFill();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    CG::Window window("Rotating Rectangle");
    window.run(drawFunc);

    return 0;
}