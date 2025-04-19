#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"

void drawSquare(CG::Vec2f center, float radius) {
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);

    CG::Vec2f offset{ radius, radius };
    CG::Vec2f bottomLeft = CG::Window::NormalizeToViewport(center - offset);
    CG::Vec2f topRight = CG::Window::NormalizeToViewport(center + offset);

    glVertex2f(bottomLeft.x, bottomLeft.y);
    glVertex2f(topRight.x, bottomLeft.y);
    glVertex2f(topRight.x, topRight.y);
    glVertex2f(bottomLeft.x, topRight.y);
    glEnd();
}


CG::Vec2f getBouncingSquarePosition(CG::Vec2f center, float radius, float speed) {
    static CG::Vec2f dir = { 0.0f, -1.0f };
    CG::Vec2f velocity = dir * speed;
    CG::Vec2f newPos = center + velocity;

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

    static CG::Vec2f center{ CG::Window::WindowSize().x / 2, CG::Window::WindowSize().y / 2 };
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