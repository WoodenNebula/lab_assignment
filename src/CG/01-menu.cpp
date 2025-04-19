#include <iostream>
#include <GL/gl.h>
#include "Utility.h"
#include "graphics/graphics.h"


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
    CG::Vec2f windowSize{ 800.0f, 800.0f };
    int window = initwindow(windowSize.x, windowSize.y, "Graphics", 0, 0, false, false);
    setcurrentwindow(window);
}

void shutdownGraphics() {
    CG::flush();
    getch();
    closegraph();
}


void drawLine() {
    CG::Vec2f p1, p2;
    std::cout << "Enter (x1, y1): ";
    std::cin >> p1;

    std::cout << "Enter (x2, y2): ";
    std::cin >> p2;

    initGraphics();
     // Draw a line
    line(p1.x, p1.y,
        p2.x, p2.y);

    shutdownGraphics();
    std::cout << "Line\n";
}

void drawTriangle() {
    CG::Vec2f top, left, right;
    std::cout << "Enter coordinates of triangle:\n";

    std::cout << "Top (x1, y1): ";
    std::cin >> top;

    std::cout << "Down Left (x2, y2): ";
    std::cin >> left;

    std::cout << "Down Right (x3, y3): ";
    std::cin >> right;

    initGraphics();
    line(left.x, left.y, right.x, right.y);
    line(left.x, left.y, top.x, top.y);
    line(top.x, top.y, right.x, right.y);

    shutdownGraphics();
    std::cout << "Triangle\n";
}

void drawRectangle() {
    CG::Vec2f bottomLeft, topRight;

    std::cout << "Down Left (x2, y2): ";
    std::cin >> bottomLeft;

    std::cout << "Top Right (x3, y3): ";
    std::cin >> topRight;

    initGraphics();
    rectangle(bottomLeft.x, topRight.y, topRight.x, bottomLeft.y);
    shutdownGraphics();
}

void drawHexagon() {
    CG::Vec2f topLeft, topRight, left, bottomLeft, bottomRight, right;

    std::cout << "Top Left: ";
    std::cin >> topLeft;

    std::cout << "Mid Left: ";
    std::cin >> left;

    std::cout << "Bottom Left: ";
    std::cin >> bottomLeft;

    std::cout << "Bottom Right: ";
    std::cin >> bottomRight;

    std::cout << "Mid Right: ";
    std::cin >> right;

    std::cout << "Top Right: ";
    std::cin >> topRight;

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
        CG::flush();
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

    CG::pause();
    CG::footer();
}