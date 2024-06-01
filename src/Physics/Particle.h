#pragma once

#include "Vec2.h"

struct Particle {
    int radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;

    Particle(float x, float y, float mass);
    ~Particle();
};
