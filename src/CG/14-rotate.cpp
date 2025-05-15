#include <iostream>
#include <GL/glut.h>

#include "graphics/graphics.h"
#include "Window.h"
#include "Polygon.h"
#include "Maths.h"

namespace CG::Maths
{
#define PI 3.14159f
Polygon translate(const Polygon& poly, const Vec2f& translation) {
    Polygon translatedPoly = poly;
    for (auto& vertex : translatedPoly.vertices) {
        vertex = vertex + translation;
    }

    translatedPoly.center = translatedPoly.center + translation;
    return translatedPoly;
}

Polygon rotate(const Polygon& polygon, float degree, const Vec2f center = Window::WindowSize() * 0.5f) {
    float angle = degree * (PI / 180.0f); // Convert to radians

    Polygon t = translate(polygon, center * -1.0f);
    Polygon r = t;

    for (int i = 0; i < polygon.vertices.size(); i++) {
        r.vertices[i] = {
            (t.vertices[i].x) * cosf(angle) - (t.vertices[i].y) * sinf(angle),
            (t.vertices[i].x) * sinf(angle) + (t.vertices[i].y) * cosf(angle),
        };
    }
    r = translate(r, center);
    r.color = { 0.0f, 1.0f, 1.0f };

    return r;
}
}; // namespace CG::Maths

using namespace CG::Maths;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    int dt = 750;

    Vec2f center = { 300.0f, 325.0f };
    Vec2f dimensions = { 200.0f, 50.0f };

    CG::Rectangle rectangle(center, dimensions, { .5f, .0f, .4f });
    rectangle.DrawFill();

    CG::Polygon r = rotate(rectangle, 30.0f);
    r.color = { 0.0f, 1.0f, 0.0f };
    r.DrawFill();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    CG::Window window("Rotating Rectangle");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    window.run(drawFunc);

    return 0;
}