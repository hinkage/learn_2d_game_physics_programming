#pragma once

#include "Body.h"
#include "MatMN.h"

class Constraint {
  public:
    Body *a;
    Body *b;

    // The anchor point in a's local space
    Vec2 aPoint;
    // The anchor point in b's local space
    Vec2 bPoint;

    virtual ~Constraint() = default;

    MatMN GetInvM() const;
    VecN GetVelocities() const;

    virtual void Solve() {}
};

class JointConstraint : public Constraint {
  private:
    MatMN jacobian;

  public:
    JointConstraint();
    JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint);
    void Solve() override;
};

class PenetrationConstraint : public Constraint {
  private:
    MatMN jacobian;
};
