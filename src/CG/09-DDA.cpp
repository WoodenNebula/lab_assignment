#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"
#include "Maths.h"

using namespace CG::Maths;

Vec2f lineStart, lineEnd;
std::vector<Vec2f> points;

void ddaLine(const Vec2f& start, const Vec2f& end) {
    Vec2f d = end - start;
    int steps = abs(d.x) > abs(d.y) ? abs(d.x) : abs(d.y);
    float xIncrement = d.x / (float)steps;
    float yIncrement = d.y / (float)steps;
    Vec2f p = start;
    for (int i = 0; i <= steps; i++) {
        points.push_back(CG::Window::NormalizeToViewport(p));
        p.x += xIncrement;
        p.y += yIncrement;
    }
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.69f, 0.0f, 1.0f);
    glPointSize(2.0f);

    glBegin(GL_POINTS);
    for (const auto& point : points) {
        glVertex2fv(&point.x);
    }
    glEnd();
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    std::cout << "Enter start point (x1, y1): ";
    std::cin >> lineStart;
    std::cout << "Enter end point (x2, y2): ";
    std::cin >> lineEnd;

    ddaLine(lineStart, lineEnd);

    CG::Window window("Direct Line");

    window.run(drawFunc);
}