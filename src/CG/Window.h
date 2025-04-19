#pragma once
#include <iostream>
#include <GL/glut.h>
#include "Utility.h"

namespace CG
{
class Window {
public:
    Window(const char* title, const CG::Vec2f& winDim = WindowSize(), const CG::Vec2f& winPos = { 0.0f, 0.0f }) {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(winDim.x, winDim.y);
        glutInitWindowPosition(winPos.x, winPos.y);
        m_windowID = glutCreateWindow(title);
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);

        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glutKeyboardFunc(keyCallback);
    }
    ~Window() { glutDestroyWindow(m_windowID); }

    /// @brief  Takes a function pointer to the display function, registers it as cb and runs main loop.
    /// @param displayFunc display function with signature `void displayFunc(void)`
    void run(void (*displayFunc)(void)) const {
        glutDisplayFunc(displayFunc);
        glutMainLoop();
    }

    int getWindowID() const { return m_windowID; }

    static Vec2f NormalizeToViewport(const Vec2f& point) {
        float x = (point.x / (float)WindowSize().x) * 2.0f - 1.0f;
        float y = (point.y / (float)WindowSize().y) * 2.0f - 1.0f;
        return { x, y };
    }
public:
    static CG::Vec2f  WindowSize() { return { 800.0f, 600.0f }; }

private:
    static void keyCallback(unsigned char key, int x, int y) {
        if (key == 27) { // ESC key
            glutDestroyWindow(glutGetWindow());
        }
    }
private:
    int m_windowID;
};
}