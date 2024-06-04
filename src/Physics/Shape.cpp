#include "Shape.h"
#include <iostream>
#include <limits>

CircleShape::CircleShape(float radius) : radius(radius) {
    std::cout << "Circle constructor" << std::endl;
}

CircleShape::~CircleShape() { std::cout << "Circle destructor" << std::endl; }

ShapeType CircleShape::GetType() const { return ShapeType::CIRCLE; }

void CircleShape::UpdateVertices(float angle, const Vec2 &position) {}

float CircleShape::GetMomentOfInertia() const {
    return (1.0f / 2.0f) * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    for (int i = 0; i < vertices.size(); i++) {
        auto &vertex = vertices[i];
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);
        // Find min and max X and Y to calculate polygon width and height
        minX = std::min(minX, vertex.x);
        maxX = std::max(maxX, vertex.x);
        minY = std::min(minY, vertex.y);
        maxY = std::max(maxY, vertex.y);
    }
    width = maxX - minX;
    height = maxY - minY;
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

float PolygonShape::FindMinSeparation(const PolygonShape &b,
                                      int &indexReferenceEdge,
                                      Vec2 &supportPoint) const {
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
            indexReferenceEdge = i;
            supportPoint = minVertex;
        }
    }
    return separation;
}

int PolygonShape::FindIncidentEdge(const Vec2 &normal) const {
    int indexIncidentEdge;
    float minProj = std::numeric_limits<float>::max();
    for (int i = 0; i < this->worldVertices.size(); i++) {
        auto edgeNormal = this->EdgeAt(i).Normal();
        auto proj = edgeNormal.Dot(normal);
        if (proj < minProj) {
            minProj = proj;
            indexIncidentEdge = i;
        }
    }
    return indexIncidentEdge;
}

int PolygonShape::ClipSegmentToLine(const std::vector<Vec2> &contactsIn,
                                    std::vector<Vec2> &contactsOut,
                                    const Vec2 &c0, const Vec2 &c1) const {
    int numOut = 0;
    Vec2 normal = (c1 - c0).Normalize();
    // Use cross product to calculate distance from point to reference edge
    float dist0 = (contactsIn[0] - c0).Cross(normal);
    float dist1 = (contactsIn[1] - c0).Cross(normal);
    if (dist0 <= 0.f) {
        contactsOut[numOut++] = contactsIn[0];
    }
    if (dist1 <= 0.f) {
        contactsOut[numOut++] = contactsOut[1];
    }
    if (dist0 * dist1 < 0.f) {
        float totalDist = dist0 - dist1;
        float t = dist0 / totalDist;
        Vec2 contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
        contactsOut[numOut++] = contact;
    }
    return numOut;
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
