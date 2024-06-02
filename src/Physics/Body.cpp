#include "Body.h"
#include "Vec2.h"
#include <iostream>

Body::Body(Shape *shape, float x, float y, float mass)
    : shape(shape), position(x, y), mass(mass), radius(1.0f),
      velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), rotation(0.f),
      angularVelocity(0.0f), angularAcceleration(0.0f), sumForces(0.0f, 0.0f),
      sumTorque(0.0f) {
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
    std::cout << "Body destructor" << std::endl;
    delete shape;
}

void Body::AddForce(const Vec2 &force) { sumForces += force; }

void Body::AddTorque(float torque) { this->sumTorque += torque; }

void Body::ClearForce() { sumForces = Vec2(0.0f, 0.0f); }

void Body::ClearTorque() { sumTorque = 0; }

void Body::IntegrateLinear(float dt) {
    acceleration = sumForces * invMass;
    // Implicit Euler
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForce();
}

void Body::IntegrateAngular(float dt) {
    angularAcceleration = sumTorque * invI;
    angularVelocity += angularAcceleration * dt;
    rotation += angularVelocity * dt;
    ClearTorque();
}
