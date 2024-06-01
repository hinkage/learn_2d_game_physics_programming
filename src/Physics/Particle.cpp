#include "Particle.h"
#include "Vec2.h"
#include <iostream>

Particle::Particle(float x, float y, float mass)
    : radius(1.0f), velocity(0.0f, 0.0f), acceleration(0.0, 0.0) {
    this->position = Vec2(x, y);
    this->mass = mass;
    std::cout << "Partical constructor" << std::endl;
}

Particle::~Particle() { std::cout << "Partical destructor" << std::endl; }
