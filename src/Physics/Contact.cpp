#include "Contact.h"

void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    float da = depth / (a->invMass + b->invMass) * a->invMass;
    float db = depth / (a->invMass + b->invMass) * b->invMass;
    a->position -= normal * da;
    b->position += normal * db;
}

void Contact::ResolveCollision() {
    ResolvePenetration();

    float e = std::min(a->restitution, b->restitution);
    Vec2 vrel = a->velocity - b->velocity;
    float impulseMagnitude =
        -(1 + e) * vrel.Dot(normal) / (a->invMass + b->invMass);
    Vec2 impulseDirection = normal;
    Vec2 jn = impulseDirection * impulseMagnitude;
    a->ApplyInpulse(jn);
    b->ApplyInpulse(-jn);
}
