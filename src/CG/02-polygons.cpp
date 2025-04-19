#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"

void drawHexagon() {
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 6; ++i) {
        float angle = 2.0f * 3.1415 * i / 6;
        glVertex2f(cos(angle) * 0.2f, sin(angle) * 0.2f);
    }
    glEnd();
}

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.3f, -0.3f);
    glVertex2f(0.3f, -0.3f);
    glVertex2f(0.0f, 0.3f);
    glEnd();
}

void drawSquare() {
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSquare();
    drawTriangle();
    drawHexagon();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    CG::Vec2i windowSize{ 800, 600 };
    CG::Vec2i windowPos{ 0, 0 };

    CG::Window window(windowSize, windowPos, "Polygon");
    window.run(drawFunc);
}