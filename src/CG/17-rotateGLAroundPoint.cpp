#include <iostream>
#include <GL/glut.h>

#include "graphics/graphics.h"
#include "Window.h"
#include "Polygon.h"
#include "Maths.h"

using namespace CG::Maths;

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    Vec2f center = { 300.0f, 325.0f };
    Vec2f dimensions = { 200.0f, 50.0f };

    glPushMatrix();
    CG::Rectangle rectangle(center, dimensions, { .5f, .0f, .4f });
    rectangle.DrawFill();

    Vec2f pivot = CG::Window::NormalizeToViewport({ 150.0f, 125.0f });

    glTranslatef(pivot.x, pivot.y, 0.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-pivot.x, -pivot.y, 0.0f);

    rectangle.color = { 0.0f, 1.0f, 0.0f };
    rectangle.DrawFill();
    glPopMatrix();

    // Draw the pivot point
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(pivot.x, pivot.y);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    CG::Window window("Rotating Rectangle Around Point (GL)");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    window.run(drawFunc);

    return 0;
}