#include <iostream>
#include <limits>
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
void drawTriangle() {}
void drawRectangle() {
    int left = 200;
    int top = 300;
    int right = 400;
    int bottom = 100;

    initGraphics();
    rectangle(left, top, right, bottom);
    shutdownGraphics();
}
void drawHexagon() {}

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