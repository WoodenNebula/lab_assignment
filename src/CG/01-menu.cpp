#include <iostream>
#include <limits>
#include <GL/gl.h>
#include "graphics/graphics.h"

struct Vec2 {
    int x;
    int y;
};

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
    Vec2 windowSize{ 800, 800 };
    int window = initwindow(windowSize.x, windowSize.y, "Graphics", 0, 0, false, false);
    setcurrentwindow(window);
}

void shutdownGraphics() {
    ignoreLine();
    getch();
    closegraph();
}


void drawLine() {
    Vec2 p1, p2;
    std::cout << "Enter (x1, y1): ";
    std::cin >> p1.x >> p1.y;

    std::cout << "Enter (x2, y2): ";
    std::cin >> p2.x >> p2.y;

    initGraphics();
     // Draw a line
    line(p1.x, p1.y,
        p2.x, p2.y);

    shutdownGraphics();
    std::cout << "Line\n";
}

void drawTriangle() {
    Vec2 top, left, right;
    std::cout << "Enter coordinates of triangle:\n";

    std::cout << "Top (x1, y1): ";
    std::cin >> top.x >> top.y;

    std::cout << "Down Left (x2, y2): ";
    std::cin >> left.x >> left.y;

    std::cout << "Down Right (x3, y3): ";
    std::cin >> right.x >> right.y;

    initGraphics();
    line(left.x, left.y, right.x, right.y);
    line(left.x, left.y, top.x, top.y);
    line(top.x, top.y, right.x, right.y);

    shutdownGraphics();
    std::cout << "Triangle\n";
}

void drawRectangle() {
    Vec2 bottomLeft, topRight;

    std::cout << "Down Left (x2, y2): ";
    std::cin >> bottomLeft.x >> bottomLeft.y;

    std::cout << "Top Right (x3, y3): ";
    std::cin >> topRight.x >> topRight.y;

    initGraphics();
    rectangle(bottomLeft.x, topRight.y, topRight.x, bottomLeft.y);
    shutdownGraphics();
}

void drawHexagon() {
    Vec2 topLeft, topRight, left, bottomLeft, bottomRight, right;

    std::cout << "Top Left: ";
    std::cin >> topLeft.x >> topLeft.y;

    std::cout << "Mid Left: ";
    std::cin >> left.x >> left.y;

    std::cout << "Bottom Left: ";
    std::cin >> bottomLeft.x >> bottomLeft.y;

    std::cout << "Bottom Right: ";
    std::cin >> bottomRight.x >> bottomRight.y;

    std::cout << "Mid Right: ";
    std::cin >> right.x >> right.y;

    std::cout << "Top Right: ";
    std::cin >> topRight.x >> topRight.y;

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