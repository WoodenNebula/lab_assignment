#include "Window.h"
#include <iostream>
#include <GL/glut.h>

#include "Maths.h"

namespace CG
{
Maths::Vec2f Window::WindowSize() { return { 800.0f, 800.0f }; }

Maths::Vec2f  Window::WindowMidPosition() {
    const float displayScale = 1.25f;
    const Maths::Vec2f screenSize = { 1920.0f, 1080.0f };
    const Maths::Vec2f windowSize = WindowSize();

    return ((screenSize * 0.5f) * (1.0f / displayScale) - windowSize * 0.5f);
}

Window::Window(const char* title, const Maths::Vec2f& winDim, const Maths::Vec2f& winPos) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winDim.x, winDim.y);
    glutInitWindowPosition(winPos.x, winPos.y);
    m_windowID = glutCreateWindow(title);
    glClearColor(0.27f, 0.27f, 0.27f, 1.0f);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glutKeyboardFunc(keyCallback);
}
Window::~Window() { glutDestroyWindow(m_windowID); }

void Window::run(void (*displayFunc)(void)) const {
    glutDisplayFunc(displayFunc);
    glutMainLoop();
}

int Window::getWindowID() const { return m_windowID; }

Maths::Vec2f Window::NormalizeToViewport(const Maths::Vec2f& point) {
    float x = (point.x / (float)WindowSize().x) * 2.0f - 1.0f;
    float y = (point.y / (float)WindowSize().y) * 2.0f - 1.0f;
    return { x, y };
}


void Window::keyCallback(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        glutDestroyWindow(glutGetWindow());
    }
}
}