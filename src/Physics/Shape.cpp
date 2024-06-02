#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(float radius) : radius(radius) {
    std::cout << "Circle constructor" << std::endl;
}

CircleShape::~CircleShape() { std::cout << "Circle destructor" << std::endl; }

ShapeType CircleShape::GetType() const { return ShapeType::CIRCLE; }

float CircleShape::GetMomentOfInertia() const {
    return (1.0f / 2.0f) * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {}

PolygonShape::~PolygonShape() {}

ShapeType PolygonShape::GetType() const { return ShapeType::POLYGON; }

float PolygonShape::GetMomentOfInertia() const { return 1.0; }

void PolygonShape::UpdateVertices(float angle, const Vec2 &position) {
    for (int i = 0; i < localVertices.size(); i++) {
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

BoxShape::BoxShape(float width, float height) : width(width), height(height) {
    localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
    localVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

    worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
    worldVertices.push_back(Vec2(-width / 2.0, +height / 2.0));
    std::cout << "Box constructor" << std::endl;
}

BoxShape::~BoxShape() { std::cout << "Box destructor" << std::endl; }

ShapeType BoxShape::GetType() const { return ShapeType::BOX; }

float BoxShape::GetMomentOfInertia() const {
    return (1.0f / 12.0f) * (width * width + height * height);
}
