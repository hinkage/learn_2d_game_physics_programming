#pragma once
#include "Body.h"

class World {
  private:
    float G;
    std::vector<Body *> bodies;
    std::vector<Vec2> forces;
    std::vector<float> torques;

  public:
    World(float gravity);
    ~World();

    void AddBody(Body *body);
    std::vector<Body *> &GetBodies();

    void AddForce(const Vec2 &force);
    void AddTorque(float torque);
    void Update(float dt, bool debug);

    void CheckCollisions(bool debug);
};
