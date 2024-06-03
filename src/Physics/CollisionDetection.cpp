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
    if (aIsPolygon && bIsCircle) {
        return IsCollidingPolygonCircle(a, b, contact);
    }
    if (aIsCircle && bIsPolygon) {
        return IsCollidingPolygonCircle(b, a, contact);
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
    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;
    float abSep = as->FindMinSeparation(*bs, aAxis, aPoint);
    float baSep = bs->FindMinSeparation(*as, bAxis, bPoint);
    if (abSep >= 0) {
        return false;
    }
    if (baSep >= 0) {
        return false;
    }
    contact.a = a;
    contact.b = b;
    if (abSep > baSep) {
        contact.depth = -abSep;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    } else {
        contact.depth = -baSep;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;
        contact.end = bPoint;
    }
    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body *polygon, Body *circle,
                                                  Contact &contact) {
    auto pS = static_cast<PolygonShape *>(polygon->shape);
    auto cS = static_cast<CircleShape *>(circle->shape);
    auto &pV = pS->worldVertices;

    bool isOutside = false;
    Vec2 minCurrVertex;
    Vec2 minNextVertex;
    float distanceCircleEdge = std::numeric_limits<float>::lowest();
    for (int i = 0; i < pV.size(); i++) {
        int curr = i;
        int next = i + 1;
        if (next >= pV.size()) {
            next = 0;
        }
        auto edge = pS->EdgeAt(curr);
        auto normal = edge.Normal();
        auto vertexToCircleCenter = circle->position - pV[curr];
        float projection = vertexToCircleCenter.Dot(normal);
        if (projection > 0.f) {
            distanceCircleEdge = projection;
            minCurrVertex = pV[curr];
            minNextVertex = pV[next];
            isOutside = true;
            break;
        } else {
            if (projection > distanceCircleEdge) {
                distanceCircleEdge = projection;
                minCurrVertex = pV[curr];
                minNextVertex = pV[next];
            }
        }
    }

    if (isOutside) {
        Vec2 v1 = circle->position - minCurrVertex;
        Vec2 v2 = minNextVertex - minCurrVertex;
        if (v1.Dot(v2) < 0.f) {
            float d = v1.Magnitude() - cS->radius;
            if (d > 0.f) {
                return false;
            }
            contact.a = polygon;
            contact.b = circle;
            contact.depth = -d;
            contact.normal = v1.Normalize();
            contact.start = circle->position + (contact.normal * -cS->radius);
            contact.end = contact.start + (contact.normal * contact.depth);
        } else {
            Vec2 v1 = circle->position - minNextVertex;
            Vec2 v2 = minCurrVertex - minNextVertex;
            if (v1.Dot(v2) < 0.f) {
                float d = v1.Magnitude() - cS->radius;
                if (d > 0.f) {
                    return false;
                }
                contact.a = polygon;
                contact.b = circle;
                contact.depth = -d;
                contact.normal = v1.Normalize();
                contact.start =
                    circle->position + (contact.normal * -cS->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            } else {
                float d = distanceCircleEdge - cS->radius;
                if (d > 0.f) {
                    return false;
                }
                contact.a = polygon;
                contact.b = circle;
                contact.depth = -d;
                contact.normal = (minNextVertex - minCurrVertex).Normal();
                contact.start =
                    circle->position - (contact.normal * cS->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
    } else {
        contact.a = polygon;
        contact.b = circle;
        // radius + distance
        contact.depth = cS->radius - distanceCircleEdge;
        contact.normal = (minNextVertex - minCurrVertex).Normal();
        contact.start = circle->position - (contact.normal * cS->radius);
        contact.end = contact.start + (contact.normal * contact.depth);
    }

    return true;
}
