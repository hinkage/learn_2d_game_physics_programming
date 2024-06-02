#pragma once

#include "Body.h"

struct CollisionDetection {
    static bool IsColliding(Body *a, Body *b);
    static bool IsCollidingCircleCircle(Body *a, Body *b);
    // static bool IsCollidingPolygonPolygon(Body *a, Body *b);
    // static bool IsCollidingPolygonCircle(Body *a, Body *b);
};
