#include "World.h"
#include "../Graphics.h"
#include "CollisionDetection.h"
#include "Constants.h"
#include <iostream>

World::World(float gravity) {
    G = -gravity;
    std::cout << "World constructor" << std::endl;
}

World::~World() {
    for (auto body : bodies) {
        delete body;
    }
    std::cout << "World destructor" << std::endl;
}

void World::AddBody(Body *body) { bodies.push_back(body); }

std::vector<Body *> &World::GetBodies() { return bodies; }

void World::AddConstraint(Constraint *constraint) {
    constraints.push_back(constraint);
}

std::vector<Constraint *> &World::GetConstraints() { return constraints; }

void World::AddForce(const Vec2 &force) { forces.push_back(force); }

void World::AddTorque(float torque) { torques.push_back(torque); }

void World::Update(float dt, bool debug) {
    for (auto body : bodies) {
        Vec2 weight = Vec2(0.f, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);

        for (auto &force : forces) {
            body->AddForce(force);
        }

        for (auto torque : torques) {
            body->AddTorque(torque);
        }
    }

    // Integrate all the forces
    for (auto body : bodies) {
        body->IntegrateForces(dt);
    }

    // Solve all constraints
    // Solve system of constraints iteratively
    for (int i = 0; i < 50; i++) {
        for (auto constraint : constraints) {
            constraint->Solve();
        }
    }

    for (auto body : bodies) {
        body->IntegrateVelocities(dt);
    }

    CheckCollisions(debug);
}

void World::CheckCollisions(bool debug) {
    for (int i = 0; i < bodies.size(); i++) {
        for (int j = i + 1; j < bodies.size(); j++) {
            Body *a = bodies[i];
            Body *b = bodies[j];
            a->isColliding = false;
            b->isColliding = false;
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) {
                contact.ResolveCollision();
                if (debug) {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y,
                                             3, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3,
                                             0xFFFF00FF);
                    Graphics::DrawLine(contact.start.x, contact.start.y,
                                       contact.start.x + contact.normal.x * 15,
                                       contact.start.y + contact.normal.y * 15,
                                       0xFFFF00FF);
                    a->isColliding = true;
                    b->isColliding = true;
                }
            }
        }
    }
}
