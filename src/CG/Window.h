#pragma once
#include <iostream>
#include <GL/glut.h>
#include "Utility.h"

namespace CG { namespace Maths {} }

namespace CG
{
class Window {
public:
    Window(const char* title,
        const CG::Maths::Vec2f& winDim = Window::WindowSize(),
        const CG::Maths::Vec2f& winPos = Window::WindowMidPosition());
    ~Window();

    /// @brief  Takes a function pointer to the display function, registers it as cb and runs main loop.
    /// @param displayFunc display function with signature `void displayFunc(void)`
    void run(void (*displayFunc)(void)) const;

    int getWindowID() const;

    static CG::Maths::Vec2f NormalizeToViewport(const CG::Maths::Vec2f& point);
    static CG::Maths::Vec2f  WindowSize();
    static CG::Maths::Vec2f  WindowMidPosition();

private:
    static void keyCallback(unsigned char key, int x, int y);
private:
    int m_windowID;
};
}