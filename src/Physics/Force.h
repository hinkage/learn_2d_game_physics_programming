#pragma once

#include "Particle.h"
#include "Vec2.h"

struct Force {
    static Vec2 GenerateDragForce(const Particle &particle, float k);
    static Vec2 GenerateFrictionForce(const Particle &particle, float k);
};
