#pragma once

#include "Particle.h"
#include "Vec2.h"

struct Force {
    static Vec2 GenerateDragForce(const Particle &particle, float k);
    static Vec2 GenerateFrictionForce(const Particle &particle, float k);
    static Vec2 GenerateGravitationalForce(const Particle &a, const Particle &b,
                                           float G, float minDistance,
                                           float maxDistance);
    static Vec2 GenerateSpringForce(const Particle &particle, Vec2 anchor,
                                    float restLength, float k);

    Vec2 GenerateSpringForce(const Particle &particle, const Particle &b,
                             float restLength, float k);
};
