#pragma once

#include "Shape.h"
#include "Vec2.h"
#include <SDL_image.h>
#include <SDL_render.h>

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
    float friction;

    Shape *shape = nullptr;
    SDL_Texture *texture = nullptr;

    Body(Shape *shape, float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2 &force);
    void AddTorque(float torque);
    void ClearForce();
    void ClearTorque();
    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);
    void IntegrateForces(const float dt);
    void IntegrateVelocities(const float dt);
    // Object with infinite mass
    bool IsStatic();
    void ApplyInpulseLinear(const Vec2 &j);
    void ApplyInpulseAngular(const float j);
    void ApplyInpulseAtPoint(const Vec2 &j, const Vec2 &r);

    void SetTexture(const char *textureFileName);

    Vec2 LocalSpaceToWorldSpace(const Vec2 &point) const;
    Vec2 WorldSpaceToLocalSpace(const Vec2 &point) const;
};
