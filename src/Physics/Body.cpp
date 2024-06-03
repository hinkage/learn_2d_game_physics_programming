#include "Body.h"
#include "../Graphics.h"
#include "Vec2.h"
#include <cmath>
#include <iostream>

Body::Body(Shape *shape, float x, float y, float mass)
    : shape(shape), position(x, y), mass(mass), velocity(0.0f, 0.0f),
      acceleration(0.0f, 0.0f), rotation(0.f), angularVelocity(0.0f),
      angularAcceleration(0.0f), sumForces(0.0f, 0.0f), sumTorque(0.0f),
      restitution(1.f), friction(0.7f) {
    if (mass != 0.0f) {
        this->invMass = 1.0f / mass;
    } else {
        this->invMass = 0.0f;
    }
    this->I = shape->GetMomentOfInertia() * mass;
    if (this->I != 0.0f) {
        this->invI = 1.0f / this->I;
    } else {
        this->invI = 0.0f;
    }
    std::cout << "Body constructor" << std::endl;
}

Body::~Body() {
    delete shape;
    SDL_DestroyTexture(texture);
    std::cout << "Body destructor" << std::endl;
}

void Body::AddForce(const Vec2 &force) { sumForces += force; }

void Body::AddTorque(float torque) { this->sumTorque += torque; }

void Body::ClearForce() { sumForces = Vec2(0.0f, 0.0f); }

void Body::ClearTorque() { sumTorque = 0; }

void Body::IntegrateForces(const float dt) {
    if (IsStatic()) {
        return;
    }
    acceleration = sumForces * invMass;
    // Implicit Euler
    velocity += acceleration * dt;

    angularAcceleration = sumTorque * invI;
    angularVelocity += angularAcceleration * dt;

    ClearForce();
    ClearTorque();
}

void Body::IntegrateVelocities(const float dt) {
    if (IsStatic()) {
        return;
    }
    position += velocity * dt;
    rotation += angularVelocity * dt;
    shape->UpdateVertices(rotation, position);
}

bool Body::IsStatic() {
    float epsilon = 0.005f;
    return fabs(invMass - 0.0f) < epsilon;
}

void Body::ApplyInpulseLinear(const Vec2 &j) {
    if (IsStatic()) {
        return;
    }
    velocity += j * invMass;
}

void Body::ApplyInpulseAngular(const float j) {
    if (IsStatic()) {
        return;
    }
    angularVelocity += j * invI;
}

void Body::ApplyInpulseAtPoint(const Vec2 &j, const Vec2 &r) {
    if (IsStatic()) {
        return;
    }
    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

void Body::SetTexture(const char *textureFileName) {
    SDL_Surface *surface = IMG_Load(textureFileName);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

Vec2 Body::LocalSpaceToWorldSpace(const Vec2 &point) const {
    Vec2 rotated = point.Rotate(rotation);
    return rotated + position;
}

Vec2 Body::WorldSpaceToLocalSpace(const Vec2 &point) const {
    float localX = point.x - position.x;
    float localY = point.y - position.y;
    float x = cos(-rotation) * localX - sin(-rotation) * localY;
    float y = sin(-rotation) * localX + cos(-rotation) * localY;
    return Vec2(x, y);
}
