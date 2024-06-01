#include "Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Particle &particle, float k) {
    Vec2 dragForce = Vec2(0.0f, 0.0f);
    auto ms = particle.velocity.MagnitudeSquared();
    if (ms > 0.0f) {
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;
        float dragMagnitude = k * ms;
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle &particle, float k) {
    Vec2 frictionForce = Vec2();
    frictionForce = (particle.velocity.UnitVector() * -1.0f) * k;
    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle &a, const Particle &b,
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

Vec2 Force::GenerateSpringForce(const Particle &particle, Vec2 anchor,
                                float restLength, float k) {
    Vec2 distance = particle.position - anchor;
    float displacement = distance.Magnitude() - restLength;
    Vec2 sprintDirection = distance.UnitVector();
    float sprintMagnitude = -k * displacement;
    return sprintDirection * sprintMagnitude;
}

Vec2 Force::GenerateSpringForce(const Particle &particle, const Particle &b,
                                float restLength, float k) {
    Vec2 distance = particle.position - b.position;
    float displacement = distance.Magnitude() - restLength;
    Vec2 sprintDirection = distance.UnitVector();
    float sprintMagnitude = -k * displacement;
    return sprintDirection * sprintMagnitude;
}
