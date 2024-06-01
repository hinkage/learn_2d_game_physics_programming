#include "Force.h"

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
