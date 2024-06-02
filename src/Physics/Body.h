#pragma once

#include "Shape.h"
#include "Vec2.h"

struct Body {
    bool isColliding = false;
    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    Vec2 sumForces;
    float sumTorque;

    float mass;
    float invMass;
    // Moment of inertia
    float I;
    float invI;

    float restitution;

    Shape *shape = nullptr;

    Body(Shape *shape, float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2 &force);
    void AddTorque(float torque);
    void ClearForce();
    void ClearTorque();
    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);
    void Update(float dt);
    // Object with infinite mass
    bool IsStatic();
    void ApplyInpulse(const Vec2 &j);
};
