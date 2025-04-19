#pragma once
#include <GL/glut.h>
#include <vector>
#include "Utility.h"
#include "Maths.h"

namespace CG
{
class Polygon {
public:
    Polygon() {};
    Polygon(const CG::Maths::Vec2f& center, const CG::Maths::Vec2f& dimensions, const CG::Maths::Vec3f& color)
        : center(center), dimensions(dimensions), color(color) {
        CG::Maths::Vec2f bottomLeft = Window::NormalizeToViewport(center - dimensions * 0.5f);
        CG::Maths::Vec2f topRight = Window::NormalizeToViewport(center + dimensions * 0.5f);

        vertices.push_back(bottomLeft);
        vertices.push_back(CG::Maths::Vec2f{ topRight.x, bottomLeft.y });
        vertices.push_back(topRight);
        vertices.push_back(CG::Maths::Vec2f{ bottomLeft.x, topRight.y });
    }

    void DrawFill() const {
        glBegin(GL_POLYGON);
        glColor3f(color.r, color.g, color.b);
        for (const auto& vertex : vertices) {
            glVertex2fv(&vertex.x);
        }
        glEnd();
    }

    void DrawOutline(float lineWidth = 1.0f, const CG::Maths::Vec3f& color = { 0.0f, 0.0f, 0.0f }) const {
        glBegin(GL_LINE_LOOP);
        glLineWidth(lineWidth);
        glColor3f(color.r, color.g, color.b);

        for (const auto& vertex : vertices) {
            glVertex2fv(&vertex.x);
        }
        glEnd();
    }
public:
    CG::Maths::Vec2f center;
    CG::Maths::Vec2f dimensions;
    CG::Maths::Vec3f color;
    std::vector<CG::Maths::Vec2f> vertices;
};


class Triangle : public Polygon {
public:
    Triangle(const CG::Maths::Vec2f& _center, const CG::Maths::Vec2f& _dimensions, const CG::Maths::Vec3f& _color)
        : Polygon() {
        center = _center;
        dimensions = _dimensions;
        color = _color;

        CG::Maths::Vec2f bottomLeft = CG::Window::NormalizeToViewport({ center.x - dimensions.x * 0.5f, center.y - dimensions.y * 0.5f });
        CG::Maths::Vec2f bottomRight = CG::Window::NormalizeToViewport({ center.x + dimensions.x * 0.5f, center.y - dimensions.y * 0.5f });
        CG::Maths::Vec2f top = CG::Window::NormalizeToViewport({ center.x, center.y + dimensions.y * 0.5f });

        vertices.push_back(bottomLeft);
        vertices.push_back(bottomRight);
        vertices.push_back(top);
    }

    void DrawFill() const {
        glBegin(GL_TRIANGLES);
        glColor3f(color.r, color.g, color.b);

        for (const auto& vertex : vertices) {
            std::cout << vertex << std::endl;
            glVertex2fv(&vertex.x);
        }
        glEnd();
    }
};


class Rectangle : public Polygon {
public:
    Rectangle(const CG::Maths::Vec2f& center, const CG::Maths::Vec2f& dimensions, const CG::Maths::Vec3f& color)
        : Polygon(center, dimensions, color) {}
};
}// namespace CG