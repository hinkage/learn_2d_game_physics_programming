#pragma once
#include "Body.h"
#include "Constraint.h"

class World {
  private:
    float G;
    std::vector<Body *> bodies;
    std::vector<Constraint *> constraints;
    std::vector<Vec2> forces;
    std::vector<float> torques;

  public:
    World(float gravity);
    ~World();

    void AddBody(Body *body);
    std::vector<Body *> &GetBodies();

    void AddConstraint(Constraint *constraint);
    std::vector<Constraint *> &GetConstraints();

    void AddForce(const Vec2 &force);
    void AddTorque(float torque);
    void Update(float dt, bool debug);
};
