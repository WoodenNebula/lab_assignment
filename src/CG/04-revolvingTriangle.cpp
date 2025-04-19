#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Polygon.h"
#include "Utility.h"

void drawTriangle(CG::Vec2f center, float radius) {
    glBegin(GL_POLYGON);

    CG::Triangle triangle(center, { radius, radius }, { 0.0f, 0.0f, 0.0f });
    CG::Vec3f colors[] = {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }
    };
    for (size_t i = 0; i < triangle.vertices.size(); i++) {
        glColor3fv(&colors[i].r);
        glVertex2fv(&triangle.vertices[i].x);
    }
    glEnd();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CG::Vec2f center{ CG::Window::WindowSize().x / 2, CG::Window::WindowSize().y / 2 };
    glRotatef(0.05f, 1.0f, 1.0f, 1.0f);
    drawTriangle(center, 300.0f);

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    CG::Window window("Revolving Triangle");
    glMatrixMode(GL_MODELVIEW);
    window.run(drawFunc);
}