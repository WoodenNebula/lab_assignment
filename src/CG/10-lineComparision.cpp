#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <GL/glut.h>

#include "Window.h"
#include "Utility.h"

CG::Vec2f lineStart, lineEnd;
std::vector<CG::Vec2f> points;

void ddaLine(const CG::Vec2f& start, const CG::Vec2f& end) {
    CG::Vec2f d = end - start;
    int steps = abs(d.x) > abs(d.y) ? abs(d.x) : abs(d.y);
    float xIncrement = d.x / (float)steps;
    float yIncrement = d.y / (float)steps;
    CG::Vec2f p = start;
    for (int i = 0; i <= steps; i++) {
        points.push_back(CG::Window::NormalizeToViewport(p));
        p.x += xIncrement;
        p.y += yIncrement;
    }
}

void bresenhamLine(const CG::Vec2f& start, const CG::Vec2f& end) {
    CG::Vec2f d = end - start;
    CG::Vec2f d2 = d * 2.0f;
    CG::Vec2f point = start;
    int p = d2.y - d.x;
    if (d.y < 0) {
        d.y = -d.y;
        d2.x = -d2.x;
    }
    if (d.x < 0) {
        d.x = -d.x;
        d2.y = -d2.y;
    }
    if (d.y <= d.x) { // Shallow slope
        while (point.x != end.x) {
            points.push_back(CG::Window::NormalizeToViewport(point));
            point.x += (end.x > start.x ? 1 : -1);
            if (p < 0) {
                p += d2.y;
            }
            else {
                p += d2.y - d2.x;
                point.y += (end.y > start.y ? 1 : -1);
            }
        }
    }
    else { // Steep slope
        while (point.y != d2.y) {
            points.push_back(CG::Window::NormalizeToViewport(point));
            point.y += (end.y > start.y ? 1 : -1);
            if (p <= 0) {
                p += d2.x;
            }
            else {
                p += d2.x - d2.y;
                point.x += (end.x > start.x ? 1 : -1);
            }
        }
    }

}

int64_t drawDirect() {
    auto startTime = std::chrono::high_resolution_clock::now();
    glBegin(GL_POINTS);
    glVertex2fv(&lineStart.x);
    glVertex2fv(&lineEnd.x);
    glEnd();
    glutSwapBuffers();
    auto endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
}

int64_t drawDDA() {
    auto    startTime = std::chrono::high_resolution_clock::now();
    ddaLine(lineStart, lineEnd);
    glBegin(GL_POINTS);
    for (const auto& point : points) {
        glVertex2fv(&point.x);
    }
    glEnd();
    glutSwapBuffers();

    auto endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
}

int64_t drawBresenham() {
    auto startTime = std::chrono::high_resolution_clock::now();

    bresenhamLine(lineStart, lineEnd);
    glBegin(GL_POINTS);
    for (const auto& point : points) {
        glVertex2fv(&point.x);
    }
    glEnd();
    glutSwapBuffers();

    auto endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
}

void compareTime(int64_t direct, int64_t dda, int64_t bresenham) {
    struct Time {
        int64_t time;
        std::string name;
    };
    std::vector<Time> times = { { direct, "Direct" }, { dda, "DDA" }, { bresenham, "Bresenham" } };
    std::sort(times.begin(), times.end(), [](const Time& a, const Time& b) { return a.time < b.time; });

    std::cout << "Comparision in nanoseconds:" << std::endl;
    for (size_t i = 0; i < times.size() - 1; i++) {
        std::cout << times[i].name << ":(" << times[i].time << ") < ";
    }
    std::cout << times[times.size() - 1].name << ":(" << times[times.size() - 1].time << ")" << std::endl;
}

void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.69f, 0.0f, 1.0f);
    glPointSize(2.0f);

    points.clear();


    int64_t direct = drawDirect();

    points.clear();

    int64_t dda = drawDDA();

    points.clear();

    int64_t bresenham = drawBresenham();

    compareTime(direct, dda, bresenham);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    std::cout << "Enter start point (x1, y1): ";
    std::cin >> lineStart;
    std::cout << "Enter end point (x2, y2): ";
    std::cin >> lineEnd;

    ddaLine(lineStart, lineEnd);

    CG::Window window("Direct Line");

    window.run(drawFunc);
}