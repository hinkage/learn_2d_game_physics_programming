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

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        auto &vertex = vertices[i];
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);
    }
    std::cout << "PolygonShape constructor" << std::endl;
}

PolygonShape::~PolygonShape() {
    std::cout << "PolygonShape destructor" << std::endl;
}

ShapeType PolygonShape::GetType() const { return ShapeType::POLYGON; }

float PolygonShape::GetMomentOfInertia() const { return 5000.0; }

void PolygonShape::UpdateVertices(float angle, const Vec2 &position) {
    for (int i = 0; i < localVertices.size(); i++) {
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

Vec2 PolygonShape::EdgeAt(int index) const {
    int currVertex = index;
    int nextVertex = index + 1;
    if (nextVertex >= worldVertices.size()) {
        nextVertex = 0;
    }
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape &b, Vec2 &axis,
                                      Vec2 &point) const {
    float separation = std::numeric_limits<float>::lowest();
    for (int i = 0; i < worldVertices.size(); i++) {
        Vec2 va = worldVertices[i];
        auto edge = EdgeAt(i);
        Vec2 normal = edge.Normal();
        float minSep = std::numeric_limits<float>::max();
        Vec2 minVertex;
        for (int j = 0; j < b.worldVertices.size(); j++) {
            Vec2 vb = b.worldVertices[j];
            float proj = (vb - va).Dot(normal);
            if (proj < minSep) {
                minSep = proj;
                minVertex = vb;
            }
        }
        if (minSep > separation) {
            separation = minSep;
            axis = edge;
            point = minVertex;
        }
    }
    return separation;
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
