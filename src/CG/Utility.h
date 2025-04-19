#pragma once
#include "Maths.h"

namespace CG
{
class Window;
class Polygon;
class Triangle;
class Rectangle;
// !forward declarations
}

namespace CG::Utility
{
CG::Maths::Vec3f getRandomColor();

void flush();
void pause();
void footer();
} // namespace Utility