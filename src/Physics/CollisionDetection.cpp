#include "CollisionDetection.h"
#include "Contact.h"
#include <limits>

bool CollisionDetection::IsColliding(Body *a, Body *b,
                                     std::vector<Contact> &contacts) {
    auto as = a->shape->GetType();
    auto bs = b->shape->GetType();
    bool aIsCircle = as == ShapeType::CIRCLE;
    bool bIsCircle = bs == ShapeType::CIRCLE;
    bool aIsPolygon = as == ShapeType::POLYGON || as == ShapeType::BOX;
    bool bIsPolygon = bs == ShapeType::POLYGON || bs == ShapeType::BOX;
    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contacts);
    }
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contacts);
    }
    if (aIsPolygon && bIsCircle) {
        return IsCollidingPolygonCircle(a, b, contacts);
    }
    if (aIsCircle && bIsPolygon) {
        return IsCollidingPolygonCircle(b, a, contacts);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(
    Body *a, Body *b, std::vector<Contact> &contacts) {
    auto aS = static_cast<CircleShape *>(a->shape);
    auto bS = static_cast<CircleShape *>(b->shape);
    const Vec2 ab = b->position - a->position;
    const float radiusSum = aS->radius + bS->radius;
    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);
    if (!isColliding) {
        return false;
    }
    Contact contact;
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.start = b->position - contact.normal * bS->radius;
    contact.end = a->position + contact.normal * aS->radius;
    contact.depth = (contact.end - contact.start).Magnitude();
    contacts.push_back(contact);
    return true;
}

bool CollisionDetection::IsCollidingPolygonPolygon(
    Body *a, Body *b, std::vector<Contact> &contacts) {
    PolygonShape *as = static_cast<PolygonShape *>(a->shape);
    PolygonShape *bs = static_cast<PolygonShape *>(b->shape);
    int aIndexReferenceEdge, bIndexReferenceEdge;
    Vec2 aSupportPoint, bSupportPoint;
    float abSep =
        as->FindMinSeparation(*bs, aIndexReferenceEdge, aSupportPoint);
    if (abSep >= 0) {
        return false;
    }
    float baSep =
        bs->FindMinSeparation(*as, bIndexReferenceEdge, bSupportPoint);
    if (baSep >= 0) {
        return false;
    }

    PolygonShape *referenceShape;
    PolygonShape *incidentShape;
    int indexReferenceEdge;
    if (abSep > baSep) {
        referenceShape = as;
        incidentShape = bs;
        indexReferenceEdge = aIndexReferenceEdge;
    } else {
        referenceShape = bs;
        incidentShape = as;
        indexReferenceEdge = bIndexReferenceEdge;
    }

    Vec2 referenceEdge = referenceShape->EdgeAt(indexReferenceEdge);
    // clipping
    int incidentIndex = incidentShape->FindIncidentEdge(referenceEdge.Normal());
    int incidentNextIndex = (incidentIndex + 1);
    if (incidentNextIndex >= incidentShape->worldVertices.size()) {
        incidentNextIndex = 0;
    }
    Vec2 v0 = incidentShape->worldVertices[incidentIndex];
    Vec2 v1 = incidentShape->worldVertices[incidentNextIndex];
    std::vector<Vec2> contactPoints = {v0, v1};
    std::vector<Vec2> clippedPoints = contactPoints;
    for (int i = 0; i < referenceShape->worldVertices.size(); i++) {
        if (i == indexReferenceEdge) {
            continue;
        }
        Vec2 c0 = referenceShape->worldVertices[i];
        int i1 = i + 1;
        if (i1 >= referenceShape->worldVertices.size()) {
            i1 = 0;
        }
        Vec2 c1 = referenceShape->worldVertices[i1];
        int numClipped = referenceShape->ClipSegmentToLine(
            contactPoints, clippedPoints, c0, c1);
        if (numClipped < 2) {
            break;
        }
        contactPoints = clippedPoints;
    }

    auto vref = referenceShape->worldVertices[indexReferenceEdge];
    for (auto &vclip : clippedPoints) {
        float sepration = (vclip - vref).Dot(referenceEdge.Normal());
        if (sepration <= 0.f) {
            Contact contact;
            contact.a = a;
            contact.b = b;
            contact.normal = referenceEdge.Normal();
            contact.start = vclip;
            contact.end = vclip + contact.normal * -sepration;
            if (baSep >= abSep) {
                std::swap(contact.start, contact.end);
                contact.normal *= -1.f;
            }
            contacts.push_back(contact);
        }
    }
    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(
    Body *polygon, Body *circle, std::vector<Contact> &contacts) {
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

    Contact contact;
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
    contacts.push_back(contact);
    return true;
}
