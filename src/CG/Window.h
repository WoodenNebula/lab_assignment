#include <iostream>
#include <GL/glut.h>
#include "Utility.h"

namespace CG
{
static CG::Vec2i windowSize{ 800, 600 };
static CG::Vec2i windowPos{ 0, 0 };
class Window {
public:
    Window(const char* title, const CG::Vec2i& winDim = windowSize, const CG::Vec2i& winPos = windowPos) {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(winDim.x, winDim.y);
        glutInitWindowPosition(winPos.x, winPos.y);
        m_windowID = glutCreateWindow(title);
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);

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