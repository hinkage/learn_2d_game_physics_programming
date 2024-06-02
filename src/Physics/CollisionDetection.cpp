#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body *a, Body *b, Contact &contact) {
    bool aIsCircle = a->shape->GetType() == ShapeType::CIRCLE;
    bool bIsCircle = b->shape->GetType() == ShapeType::CIRCLE;
    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body *a, Body *b,
                                                 Contact &contact) {
    auto aS = static_cast<CircleShape *>(a->shape);
    auto bS = static_cast<CircleShape *>(b->shape);
    const Vec2 ab = b->position - a->position;
    const float radiusSum = aS->radius + bS->radius;
    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);
    if (!isColliding) {
        return false;
    }
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.start = b->position - contact.normal * bS->radius;
    contact.end = a->position + contact.normal * aS->radius;
    contact.depth = (contact.end - contact.start).Magnitude();
    return true;
}
