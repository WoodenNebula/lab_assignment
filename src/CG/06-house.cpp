#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"
#include "Polygon.h"


void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CG::Vec2f wallDimensions = { 300.0f, 250.0f };
    CG::Vec2f windowDimensions = { 75.0f, 75.0f };
    CG::Vec2f doorDimensions = { 80.0f, 100.0f };
    CG::Vec2f roofDimensions = { wallDimensions.x, 100.0f };

    CG::Rectangle wall(
        { CG::Window::WindowSize().x / 2, CG::Window::WindowSize().y / 2 },
        wallDimensions,
        { 0.0f, 1.0f, 0.0f });
    CG::Rectangle windowLeft(
        { wall.center.x - wall.dimensions.x / 4.0f, wall.center.y + wall.dimensions.y / 5.8f },
        windowDimensions,
        { 1.0f, 0.0f, 0.0f });
    CG::Rectangle windowRight(
        { wall.center.x + wall.dimensions.x / 4.0f, wall.center.y + wall.dimensions.y / 5.8f },
        windowDimensions,
        { 1.0f, 0.0f, 0.0f });
    CG::Rectangle door(
        { wall.center.x, wall.center.y - wall.dimensions.y / 4.0f },
        doorDimensions,
        { 0.35f, 0.5f, 0.69f });
    CG::Triangle roof(
        { wall.center.x, wall.center.y + wall.dimensions.y / 2.0f + roofDimensions.y / 2.0f },
        roofDimensions,
        { 0.35f, 0.5f, 0.69f });


    wall.DrawFill();
    wall.DrawOutline();
    windowLeft.DrawFill();
    windowLeft.DrawOutline();
    windowRight.DrawFill();
    windowRight.DrawOutline();
    door.DrawFill();
    door.DrawOutline();
    roof.DrawFill();
    roof.DrawOutline();

    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    CG::Window window("House");

    window.run(drawFunc);
}