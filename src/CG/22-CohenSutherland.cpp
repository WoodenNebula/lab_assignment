#include <iostream>
#include <GL/glut.h>
#include "graphics/graphics.h"
#include "Window.h"
#include "Maths.h"

using namespace CG::Maths;

// Cohen-Sutherland region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Clipping rectangle boundaries (in pixel space)
float xmin, xmax, ymin, ymax;

int computeCode(const Vec2f& p) {
    int code = INSIDE;
    if (p.x < xmin) code |= LEFT;
    else if (p.x > xmax) code |= RIGHT;
    if (p.y < ymin) code |= BOTTOM;
    else if (p.y > ymax) code |= TOP;
    return code;
}

bool cohenSutherlandClip(Vec2f& p0, Vec2f& p1) {
    int code0 = computeCode(p0);
    int code1 = computeCode(p1);
    bool accept = false;

    while (true) {
        if ((code0 | code1) == 0) {
            accept = true;
            break;
        }
        else if (code0 & code1) {
            break;
        }
        else {
            int codeOut = code0 ? code0 : code1;
            float x, y;
            if (codeOut & TOP) {
                x = p0.x + (p1.x - p0.x) * (ymax - p0.y) / (p1.y - p0.y);
                y = ymax;
            }
            else if (codeOut & BOTTOM) {
                x = p0.x + (p1.x - p0.x) * (ymin - p0.y) / (p1.y - p0.y);
                y = ymin;
            }
            else if (codeOut & RIGHT) {
                y = p0.y + (p1.y - p0.y) * (xmax - p0.x) / (p1.x - p0.x);
                x = xmax;
            }
            else { // LEFT
                y = p0.y + (p1.y - p0.y) * (xmin - p0.x) / (p1.x - p0.x);
                x = xmin;
            }
            if (codeOut == code0) {
                p0.x = x; p0.y = y;
                code0 = computeCode(p0);
            }
            else {
                p1.x = x; p1.y = y;
                code1 = computeCode(p1);
            }
        }
    }
    return accept;
}

Vec2f lineStart, lineEnd;
bool showClipped = false;

void drawClippingWindow() {
    glColor3f(1, 1, 1);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    Vec2f bl = { xmin, ymin };
    Vec2f br = { xmax, ymin };
    Vec2f tr = { xmax, ymax };
    Vec2f tl = { xmin, ymax };
    for (auto v : { bl, br, tr, tl }) {
        auto n = CG::Window::NormalizeToViewport(v);
        glVertex2f(n.x, n.y);
    }
    glEnd();
}

void drawLine(const Vec2f& a, const Vec2f& b, float r, float g, float bcol, float width = 3.0f) {
    glColor3f(r, g, bcol);
    glLineWidth(width);
    glBegin(GL_LINES);
    auto na = CG::Window::NormalizeToViewport(a);
    auto nb = CG::Window::NormalizeToViewport(b);
    glVertex2f(na.x, na.y);
    glVertex2f(nb.x, nb.y);
    glEnd();
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawClippingWindow();
    if (!showClipped) {
        // unclipped: red
        drawLine(lineStart, lineEnd, 1, 0, 0);
    }
    else {
        Vec2f c0 = lineStart, c1 = lineEnd;
        if (cohenSutherlandClip(c0, c1)) {
            // original (faint)
            drawLine(lineStart, lineEnd, 1, 0, 0, 2.0f);
            // clipped: green
            drawLine(c0, c1, 0, 1, 0, 4.0f);
        }
        else {
            // original (faint)
            drawLine(lineStart, lineEnd, 1, 0, 0, 2.0f);
        }
    }
    glutSwapBuffers();
}

void timer(int value) {
    showClipped = true;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    CG::Window window("Cohen-Sutherland Line Clipping");

    // Set up a clipping window in pixel space
    Vec2f winSize = CG::Window::WindowSize();
    xmin = winSize.x * 0.25f;
    xmax = winSize.x * 0.75f;
    ymin = winSize.y * 0.25f;
    ymax = winSize.y * 0.75f;

    lineStart = { winSize.x * 0.1f, winSize.y * 0.1f };
    lineEnd = { winSize.x * 0.9f, winSize.y * 0.9f };

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutTimerFunc(1200, timer, 0);
    window.run(drawFunc);
    return 0;
}
