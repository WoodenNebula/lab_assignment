#include <iostream>
#include <limits>
#include <GL/gl.h>
#include "graphics/graphics.h"

void ignoreLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

enum E_MenuOptions {
    None = -1,
    Line = 1,
    Rect,
    Triangle,
    Hexagon,
    Quit,
};

void printMenu() {
    std::cout
        << "Pick an option: \n"
        << "1. Line\n"
        << "2. Rectangle\n"
        << "3. Triangle\n"
        << "4. Hexagon\n"
        << "5. Quit\n";
}

void initGraphics() {
    int window = initwindow(800, 600, "Graphics", 0, 0, 0, 0);
    setcurrentwindow(window);
}

void shutdownGraphics() {
    ignoreLine();
    getch();
    closegraph();
}

struct Vec2 {
    int x;
    int y;
};

void drawLine() {
    Vec2 p1{ 100, 200 };
    Vec2 p2{ 300, 300 };

    initGraphics();
     // Draw a line
    line(p1.x, p1.y,
        p2.x, p2.y);

    shutdownGraphics();
    std::cout << "Line\n";
}

void drawTriangle() {
    Vec2 top{ 300, 400 };
    Vec2 left{ 100, 100 };
    Vec2 right{ 600, 100 };


    initGraphics();
    line(left.x, left.y, right.x, right.y);
    line(left.x, left.y, top.x, top.y);
    line(top.x, top.y, right.x, right.y);

    shutdownGraphics();
    std::cout << "Triangle\n";
}

void drawRectangle() {
    Vec2 leftBottom{ 200, 100 };
    Vec2 rightTop{ 400, 300 };

    initGraphics();
    rectangle(leftBottom.x, rightTop.y, rightTop.x, leftBottom.x);
    shutdownGraphics();
}
void drawHexagon() {
    Vec2 topLeft{ 300, 100 };
    Vec2 topRight{ 500, 100 };
    Vec2 left{ 200, 200 };
    Vec2 bottomLeft{ 300, 300 };
    Vec2 bottomRight{ 500, 300 };
    Vec2 right{ 600, 200 };

    initGraphics();
    line(topLeft.x, topLeft.y, left.x, left.y);
    line(left.x, left.y, bottomLeft.x, bottomLeft.y);
    line(bottomLeft.x, bottomLeft.y, bottomRight.x, bottomRight.y);
    line(bottomRight.x, bottomRight.y, right.x, right.y);
    line(right.x, right.y, topRight.x, topRight.y);
    line(topRight.x, topRight.y, topLeft.x, topLeft.y);

    shutdownGraphics();
    std::cout << "Triangle\n";
}

E_MenuOptions processInput(E_MenuOptions input) {
    switch (input) {
    case E_MenuOptions::Line:
        drawLine();
        break;

    case E_MenuOptions::Rect:
        drawRectangle();
        std::cout << "Rectangle\n";
        break;

    case E_MenuOptions::Triangle:
        std::cout << "Triangle\n";
        drawTriangle();
        break;

    case E_MenuOptions::Hexagon:
        std::cout << "Hexagon\n";
        drawHexagon();
        break;

    case E_MenuOptions::Quit:
        std::cout << "Exiting!\n";
        return E_MenuOptions::Quit;
    default:
        std::cerr << "Bad option!\n";
        ignoreLine();
        std::cin.get();
    }
    return E_MenuOptions::None;
}


int main() {
    int input;
    bool shouldQuit = false;

    while (!shouldQuit) {
        system("cls");
        printMenu();
        std::cin >> input;

        shouldQuit = processInput((E_MenuOptions)input) == E_MenuOptions::Quit;
    }

    system("pause");
}