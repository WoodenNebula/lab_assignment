#include <iostream>
#include <GL/glut.h>
#include <string>

#include "graphics/graphics.h"
#include "Window.h"
#include "Polygon.h"
#include "Maths.h"

using namespace CG::Maths;

void drawRectangle(const Vec3f& color, const Vec2f& center, const Vec2f& dimensions) {
    CG::Rectangle rect(center, dimensions, color);
    rect.DrawFill();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vec2f winSize = CG::Window::WindowSize();
    Vec2f rectCenter = winSize * 0.5f;
    Vec2f rectDim = { winSize.x * 0.5f, winSize.y * 0.25f };

    // --- First viewport: left half ---
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawRectangle({ 1.0f, 0.0f, 0.0f }, rectCenter, rectDim);

    // --- Second viewport: right half ---
    glViewport(glutGet(GLUT_WINDOW_WIDTH) / 2, 0, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawRectangle({ 0.0f, 0.0f, 1.0f }, rectCenter, rectDim);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    CG::Window window("Rectangle in Two Viewports");

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    window.run(drawFunc);
    return 0;
}
