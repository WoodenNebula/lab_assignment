#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"
#include "Maths.h"

using namespace CG::Maths;

void drawSquare(Vec2f center, float radius) {
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);

    Vec2f offset{ radius, radius };
    Vec2f bottomLeft = CG::Window::NormalizeToViewport(center - offset);
    Vec2f topRight = CG::Window::NormalizeToViewport(center + offset);

    glVertex2f(bottomLeft.x, bottomLeft.y);
    glVertex2f(topRight.x, bottomLeft.y);
    glVertex2f(topRight.x, topRight.y);
    glVertex2f(bottomLeft.x, topRight.y);
    glEnd();
}


Vec2f getBouncingSquarePosition(Vec2f center, float radius, float speed) {
    static Vec2f dir = { 0.0f, -1.0f };
    Vec2f velocity = dir * speed;
    Vec2f newPos = center + velocity;

    // bounds checking
    if (newPos.y + radius > CG::Window::WindowSize().y || newPos.y - radius < 0) {
        dir = dir * -1.0f; // reverse direction};
        newPos = center + dir * speed;
        std::cout << "Velocity: " << velocity << std::endl;
    }
    return newPos;
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static Vec2f center{ CG::Window::WindowSize().x / 2, CG::Window::WindowSize().y / 2 };
    float speed = 0.15f;
    center = getBouncingSquarePosition(center, 50.0f, speed);
    drawSquare(center, 50.0f);

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    CG::Window window("Bouncing Square");
    window.run(drawFunc);
}