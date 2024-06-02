#include "Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Body &body, float k) {
    Vec2 dragForce = Vec2(0.0f, 0.0f);
    auto ms = body.velocity.MagnitudeSquared();
    if (ms > 0.0f) {
        Vec2 dragDirection = body.velocity.UnitVector() * -1.0;
        float dragMagnitude = k * ms;
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body &body, float k) {
    Vec2 frictionForce = Vec2();
    frictionForce = (body.velocity.UnitVector() * -1.0f) * k;
    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body &a, const Body &b,
                                       float G, float minDistance,
                                       float maxDistance) {
    Vec2 d = (b.position - a.position);
    float distanceSquared = d.MagnitudeSquared();
    Vec2 attractionDirection = d.UnitVector();
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);
    float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;
    Vec2 attractionForce = attractionDirection * attractionMagnitude;
    return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Body &body, Vec2 anchor,
                                float restLength, float k) {
    Vec2 distance = body.position - anchor;
    float displacement = distance.Magnitude() - restLength;
    Vec2 sprintDirection = distance.UnitVector();
    float sprintMagnitude = -k * displacement;
    return sprintDirection * sprintMagnitude;
}

Vec2 Force::GenerateSpringForce(const Body &a, const Body &b,
                                float restLength, float k) {
    Vec2 distance = a.position - b.position;
    float displacement = distance.Magnitude() - restLength;
    Vec2 sprintDirection = distance.UnitVector();
    float sprintMagnitude = -k * displacement;
    return sprintDirection * sprintMagnitude;
}
