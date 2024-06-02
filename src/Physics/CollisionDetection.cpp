#include "CollisionDetection.h"
#include <limits>

bool CollisionDetection::IsColliding(Body *a, Body *b, Contact &contact) {
    auto as = a->shape->GetType();
    auto bs = b->shape->GetType();
    bool aIsCircle = as == ShapeType::CIRCLE;
    bool bIsCircle = bs == ShapeType::CIRCLE;
    bool aIsPolygon = as == ShapeType::POLYGON || as == ShapeType::BOX;
    bool bIsPolygon = bs == ShapeType::POLYGON || bs == ShapeType::BOX;
    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contact);
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body *a, Body *b,
                                                   Contact &contact) {
    const PolygonShape *as = static_cast<PolygonShape *>(a->shape);
    const PolygonShape *bs = static_cast<PolygonShape *>(b->shape);
    if (as->FindMinSeparation(*bs) >= 0) {
        return false;
    }
    if (bs->FindMinSeparation(*as) >= 0) {
        return false;
    }
    return true;
}
