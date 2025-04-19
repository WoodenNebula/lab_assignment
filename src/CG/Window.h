#include <iostream>
#include <GL/glut.h>
#include "Utility.h"

namespace CG
{
class Window {
public:
    Window(const CG::Vec2i& winDim, const CG::Vec2i& winPos, const char* title) {
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