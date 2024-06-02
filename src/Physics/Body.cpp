#include "Body.h"
#include "Vec2.h"
#include <iostream>

Body::Body(float x, float y, float mass)
    : radius(1.0f), velocity(0.0f, 0.0f), acceleration(0.0, 0.0) {
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
    std::cout << "Partical constructor" << std::endl;
}

Body::~Body() { std::cout << "Partical destructor" << std::endl; }

void Body::AddForce(const Vec2 &force) { sumForces += force; }

void Body::ClearForce() { sumForces = Vec2(0.0f, 0.0f); }

void Body::Integrate(float dt) {
    acceleration = sumForces * invMass;
    // Implicit Euler
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForce();
}
