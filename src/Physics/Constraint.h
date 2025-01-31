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

    virtual void PreSolve(const float dt) {}
    virtual void Solve() {}
    virtual void PostSolve() {}
};

class JointConstraint : public Constraint {
  private:
    MatMN jacobian;
    VecN cachedLambda;
    float bias;

  public:
    JointConstraint();
    JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint);
    void PreSolve(const float dt) override;
    void Solve() override;
    void PostSolve() override;
};

class PenetrationConstraint : public Constraint {
  private:
    MatMN jacobian;
    VecN cachedLambda;
    float bias;
    // Normal of penetration in A's local space
    Vec2 normal;
    // Friction coefficient
    float friction;

  public:
    PenetrationConstraint();
    PenetrationConstraint(Body *a, Body *b, const Vec2 &aCollisionPoint,
                          const Vec2 &bCollisonPoint, const Vec2 &normal);
    void PreSolve(const float dt) override;
    void Solve() override;
    void PostSolve() override;
};
