#include <iostream>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"
#include "Maths.h"

using namespace CG::Maths;

Vec2f lineStart, lineEnd;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.69f, 0.0f, 1.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glVertex2fv(&lineStart.x);
    glVertex2fv(&lineEnd.x);
    glEnd();
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    std::cout << "Enter start point (x1, y1): ";
    std::cin >> lineStart;
    std::cout << "Enter end point (x2, y2): ";
    std::cin >> lineEnd;
    lineStart = CG::Window::NormalizeToViewport(lineStart);
    lineEnd = CG::Window::NormalizeToViewport(lineEnd);

    CG::Window window("Direct Line");

    window.run(drawFunc);
}
