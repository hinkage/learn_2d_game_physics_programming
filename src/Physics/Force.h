#pragma once

#include "Body.h"
#include "Vec2.h"

struct Force {
    static Vec2 GenerateDragForce(const Body &body, float k);
    static Vec2 GenerateFrictionForce(const Body &body, float k);
    static Vec2 GenerateGravitationalForce(const Body &a, const Body &b,
                                           float G, float minDistance,
                                           float maxDistance);
    static Vec2 GenerateSpringForce(const Body &body, Vec2 anchor,
                                    float restLength, float k);

    static Vec2 GenerateSpringForce(const Body &a, const Body &b,
                                    float restLength, float k);
};
