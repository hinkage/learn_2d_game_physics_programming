#pragma once

#include "Vec2.h"

struct Body {
    int radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;

    float mass;
    float invMass;

    Body(float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2& force);
    void ClearForce();
    void Integrate(float dt);
};
