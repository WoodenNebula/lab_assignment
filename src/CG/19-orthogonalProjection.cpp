#include <iostream>
#include <GL/glut.h>
#include <string>

#include "graphics/graphics.h"
#include "Window.h"
#include "Polygon.h"
#include "Maths.h"

using namespace CG::Maths;

void drawCube() {
    // Draw a colored cube centered at origin
    glBegin(GL_QUADS);
    // Front face (z = 0.5)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.3f, -0.3f, 0.3f);
    glVertex3f(0.3f, -0.3f, 0.3f);
    glVertex3f(0.3f, 0.3f, 0.3f);
    glVertex3f(-0.3f, 0.3f, 0.3f);
    // Back face (z = -0.3)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.3f, -0.3f, -0.3f);
    glVertex3f(-0.3f, 0.3f, -0.3f);
    glVertex3f(0.3f, 0.3f, -0.3f);
    glVertex3f(0.3f, -0.3f, -0.3f);
    // Left face
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.3f, -0.3f, -0.3f);
    glVertex3f(-0.3f, -0.3f, 0.3f);
    glVertex3f(-0.3f, 0.3f, 0.3f);
    glVertex3f(-0.3f, 0.3f, -0.3f);
    // Right face
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.3f, -0.3f, -0.3f);
    glVertex3f(0.3f, 0.3f, -0.3f);
    glVertex3f(0.3f, 0.3f, 0.3f);
    glVertex3f(0.3f, -0.3f, 0.3f);
    // Top face
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.3f, 0.3f, -0.3f);
    glVertex3f(-0.3f, 0.3f, 0.3f);
    glVertex3f(0.3f, 0.3f, 0.3f);
    glVertex3f(0.3f, 0.3f, -0.3f);
    // Bottom face
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-0.3f, -0.3f, -0.3f);
    glVertex3f(0.3f, -0.3f, -0.3f);
    glVertex3f(0.3f, -0.3f, 0.3f);
    glVertex3f(-0.3f, -0.3f, 0.3f);
    glEnd();
}

void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawOrthogonalCube() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -2, 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(-0.6f, 0.0f, 0.0f); // Move left
    drawCube();
    glPopMatrix();
}

void drawPerspectiveCube() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.6f, 0.0f, -2.0f); // Move right and back

    drawCube();
    glPopMatrix();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glColor3f(1, 1, 1);
    drawText(-0.95f, 0.8f, "Orthogonal Projection");
    drawOrthogonalCube();

    glMatrixMode(GL_MODELVIEW);
    glColor3f(1, 1, 1);
    drawText(0.25f, 0.8f, "Perspective Projection");
    drawPerspectiveCube();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    CG::Window window("Orthogonal Projection Demo");

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set up orthogonal projection (left, right, bottom, top, near, far)
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    window.run(drawFunc);
    return 0;
}
