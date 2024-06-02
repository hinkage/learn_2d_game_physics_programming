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

    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    Vec2 va = a->velocity +
              Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity +
              Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
    Vec2 vrel = va - vb;

    float vrelDotN = vrel.Dot(normal);

    float raCN = ra.Cross(normal);
    float rbCN = rb.Cross(normal);
    float impulseMagnitude =
        -(1 + e) * vrelDotN /
        ((a->invMass + b->invMass) + (raCN * raCN * a->invI) +
         (rbCN * rbCN * b->invI));

    Vec2 impulseDirection = normal;
    Vec2 jn = impulseDirection * impulseMagnitude;

    a->ApplyInpulse(jn, ra);
    b->ApplyInpulse(-jn, rb);
}
