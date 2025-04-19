#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"

void drawTriangle(CG::Vec2f center, float radius) {
    glBegin(GL_POLYGON);

    CG::Vec2f offset{ radius, radius };
    CG::Vec2f bottomLeft = CG::Window::NormalizeToViewport(center - offset);
    CG::Vec2f bottomRight = CG::Window::NormalizeToViewport({ center.x + offset.x, center.y - offset.y });
    CG::Vec2f top = CG::Window::NormalizeToViewport({ center.x, center.y + offset.y });

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(bottomLeft.x, bottomLeft.y);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(bottomRight.x, bottomRight.y);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(top.x, top.y);
    glEnd();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CG::Vec2f center{ CG::Window::WindowSize().x / 2, CG::Window::WindowSize().y / 2 };
    glRotatef(0.05f, 1.0f, 1.0f, 1.0f);
    drawTriangle(center, 100.0f);


    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    CG::Window window("Revolving Triangle");
    glMatrixMode(GL_MODELVIEW);
    window.run(drawFunc);
}