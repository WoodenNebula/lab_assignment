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


static CG::Vec2f center;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float speed = 0.15f;
    drawSquare(center, 50.0f);

    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    CG::Window window("Magic Square");
    center = { CG::Window::WindowSize().x / 2, CG::Window::WindowSize().y / 2 };

    glutMouseFunc([](int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Update the center position to the click position
            center = { (float)x, (float)(CG::Window::WindowSize().y - y) };
            glutPostRedisplay();
        }
        });
    window.run(drawFunc);
}