#pragma once

#include "Vec2.h"
#include <vector>

enum ShapeType : int { CIRCLE, POLYGON, BOX };

struct Shape {
    virtual ~Shape() = default;
    virtual ShapeType GetType() const = 0;
    virtual void UpdateVertices(float angle, const Vec2 &position) = 0;
    virtual float GetMomentOfInertia() const = 0;
};

struct CircleShape : public Shape {
    float radius;

    CircleShape(const float radius);
    virtual ~CircleShape();
    ShapeType GetType() const override;
    void UpdateVertices(float angle, const Vec2 &position) override;

    float GetMomentOfInertia() const override;
};

struct PolygonShape : public Shape {
    std::vector<Vec2> localVertices;
    std::vector<Vec2> worldVertices;

    PolygonShape() = default;
    PolygonShape(const std::vector<Vec2> vertices);
    virtual ~PolygonShape();
    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    // Rotate and translate vertices from local space to world space
    void UpdateVertices(float angle, const Vec2 &position) override;

    Vec2 EdgeAt(int index) const;
    float FindMinSeparation(const PolygonShape &b, int &indexReferenceEdge,
                            Vec2 &supportPoint) const;
    int FindIncidentEdge(const Vec2 &normal) const;

    int ClipSegmentToLine(const std::vector<Vec2> &contactsIn,
                          std::vector<Vec2> &contactsOut, const Vec2 &c0,
                          const Vec2 &c1) const;
};

struct BoxShape : public PolygonShape {
    float width;
    float height;

    BoxShape(float width, float height);
    virtual ~BoxShape();
    ShapeType GetType() const override;

    float GetMomentOfInertia() const override;
};
