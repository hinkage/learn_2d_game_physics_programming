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
    for (auto constraint : constraints) {
        delete constraint;
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
    std::vector<PenetrationConstraint> penetrations;

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

    for (int i = 0; i < bodies.size(); i++) {
        for (int j = i + 1; j < bodies.size(); j++) {
            Body *a = bodies[i];
            Body *b = bodies[j];
            std::vector<Contact> contacts;
            if (CollisionDetection::IsColliding(a, b, contacts)) {
                // contact.ResolveCollision();
                for (auto contact : contacts) {
                    auto pen = PenetrationConstraint(contact.a, contact.b,
                                                     contact.start, contact.end,
                                                     contact.normal);
                    penetrations.push_back(pen);

                    if (debug) {
                        Graphics::DrawFillCircle(
                            contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                        Graphics::DrawFillCircle(contact.end.x, contact.end.y,
                                                 3, 0xFFFF00FF);
                        Graphics::DrawLine(
                            contact.start.x, contact.start.y,
                            contact.start.x + contact.normal.x * 15,
                            contact.start.y + contact.normal.y * 15,
                            0xFFFF00FF);
                    }
                }
            }
        }
    }

    // Solve all constraints
    for (auto constraint : constraints) {
        constraint->PreSolve(dt);
    }
    for (auto &constraint : penetrations) {
        constraint.PreSolve(dt);
    }
    // Solve system of constraints iteratively
    for (int i = 0; i < 5; i++) {
        for (auto constraint : constraints) {
            constraint->Solve();
        }
        for (auto &constraint : penetrations) {
            constraint.Solve();
        }
    }

    for (auto body : bodies) {
        body->IntegrateVelocities(dt);
    }
}
