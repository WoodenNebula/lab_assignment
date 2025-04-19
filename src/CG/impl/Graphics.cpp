#include "Utility.h"
#include "Graphics.h"
#include "Window.h"
#include "graphics/graphics.h"
#include "Maths.h"

namespace CG::Graphics
{
void InitGraphics(const char* title) {
    CG::Maths::Vec2f windowSize = CG::Window::WindowSize();
    CG::Maths::Vec2f windowPos = CG::Window::WindowMidPosition();
    int window = initwindow(windowSize.x, windowSize.y, title, windowPos.x, windowPos.y, false, false);
    setcurrentwindow(window);
}

void ShutdownGraphics() {
    CG::Utility::flush();
    getch();
    closegraph();
}
} // namespace CG::Graphics