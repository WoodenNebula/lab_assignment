#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Polygon.h"
#include "Utility.h"

static CG::Vec2f center;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CG::Rectangle square(center, { 100.0f, 100.0f }, { 1.0f, 0.0f, 0.0f });
    square.DrawFill();
    square.DrawOutline();

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