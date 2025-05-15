#include <iostream>
#include <GL/glut.h>

#include "graphics/graphics.h"
#include "Window.h"
#include "Polygon.h"
#include "Maths.h"

namespace CG::Maths
{
Vec2f scale(const Vec2f& point, const Vec2f& scale) {
    return { point.x * scale.x, point.y * scale.y };
}
};

using namespace CG::Maths;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    int dt = 750;

    Vec2f center = { 300.0f, 325.0f };
    Vec2f dimensions = { 200.0f, 50.0f };

    CG::Rectangle rectangle(center, dimensions, { .5f, .0f, .4f });
    rectangle.DrawFill();

    for (int i = 0; i < rectangle.vertices.size(); i++) {
        rectangle.vertices[i] = scale(rectangle.vertices[i], { 2.0f, 2.0f });
    }

    rectangle.color = { 0.0f, 1.0f, 0.0f };
    rectangle.DrawOutline(20.0f, rectangle.color);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    CG::Window window("Scaled Rectangle");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    window.run(drawFunc);

    return 0;
}