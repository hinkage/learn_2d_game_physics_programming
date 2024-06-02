#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body *a, Body *b) {
    bool aIsCircle = a->shape->GetType() == ShapeType::CIRCLE;
    bool bIsCircle = b->shape->GetType() == ShapeType::CIRCLE;
    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body *a, Body *b) {
    auto aS = static_cast<CircleShape *>(a->shape);
    auto bS = static_cast<CircleShape *>(b->shape);
    const Vec2 ab = b->position - a->position;
    const float radiusSum = aS->radius + bS->radius;
    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);
    return isColliding;
}
