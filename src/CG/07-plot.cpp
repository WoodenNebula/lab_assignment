#include <iostream>
#include <vector>
#include <ctime>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"
#include "Maths.h"

using namespace CG::Maths;

std::vector<Vec2f> points;

void generateRandomPoints(int count) {
    points.clear();
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(rand() % static_cast<int>(CG::Window::WindowSize().x));
        float y = static_cast<float>(rand() % static_cast<int>(CG::Window::WindowSize().y));
        points.push_back({ x, y });
    }
}

void drawPoint(const Vec2f& point, float size = 10.0f) {
    glPointSize(size);
    auto color = CG::Utility::getRandomColor();
    glColor3fv(&color.r);
    glBegin(GL_POINTS);
    Vec2f normalizedPoint = CG::Window::NormalizeToViewport(point);
    glVertex2f(normalizedPoint.x, normalizedPoint.y);
    glEnd();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    generateRandomPoints(50);
    for (const auto& point : points) {
        drawPoint(point);
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    glutInit(&argc, argv);
    CG::Window window("Random Points Plot");

    generateRandomPoints(50);
    window.run(drawFunc);
}