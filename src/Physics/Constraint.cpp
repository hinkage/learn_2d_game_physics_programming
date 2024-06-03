#include "Constraint.h"

MatMN Constraint::GetInvM() const {
    MatMN invM(6, 6);
    invM.Zero();

    invM.rows[0][0] = a->invMass;
    invM.rows[1][1] = a->invMass;
    invM.rows[2][2] = a->invI;

    invM.rows[3][3] = b->invMass;
    invM.rows[4][4] = b->invMass;
    invM.rows[5][5] = b->invI;
    return invM;
}

VecN Constraint::GetVelocities() const {
    VecN v(6);
    v.Zero();
    v[0] = a->velocity.x;
    v[1] = a->velocity.y;
    v[2] = a->angularVelocity;
    v[3] = b->velocity.x;
    v[4] = b->velocity.y;
    v[5] = b->angularVelocity;
    return v;
}
JointConstraint::JointConstraint()
    : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.f) {
    cachedLambda.Zero();
}

JointConstraint::JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint)
    : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.f) {
    cachedLambda.Zero();
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(anchorPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(anchorPoint);
}

void JointConstraint::PreSolve(const float dt) {
    Vec2 pa = a->LocalSpaceToWorldSpace(aPoint);
    Vec2 pb = b->LocalSpaceToWorldSpace(bPoint);
    Vec2 ra = pa - a->position;
    Vec2 rb = pb - b->position;

    jacobian.Zero();

    Vec2 J1 = (pa - pb) * 2.f;
    jacobian.rows[0][0] = J1.x;
    jacobian.rows[0][1] = J1.y;

    float J2 = ra.Cross(pa - pb) * 2.f;
    jacobian.rows[0][2] = J2;

    Vec2 J3 = (pb - pa) * 2.f;
    jacobian.rows[0][3] = J3.x;
    jacobian.rows[0][4] = J3.y;

    float J4 = rb.Cross(pb - pa) * 2.f;
    jacobian.rows[0][5] = J4;

    // Warm Start (apply cached lambda)
    MatMN Jt = jacobian.Transpose();
    VecN impulse = Jt * cachedLambda;
    a->ApplyInpulseLinear(Vec2(impulse[0], impulse[1]));
    a->ApplyInpulseAngular(impulse[2]);
    b->ApplyInpulseLinear(Vec2(impulse[3], impulse[4]));
    b->ApplyInpulseAngular(impulse[5]);

    // Compute the bias term (baumgarte stabilization)
    const float beta = 0.1f;
    auto ab = pb - pa;
    float C = ab.Dot(ab);
    C = std::max(0.f, C - 0.01f);
    bias = (beta / dt) * C;
}

void JointConstraint::Solve() {
    VecN V = GetVelocities();
    MatMN invM = GetInvM();

    MatMN &J = jacobian;
    MatMN Jt = J.Transpose();

    // Ax = b
    MatMN A = J * invM * Jt;
    VecN vb = J * V * -1.f;
    vb[0] -= bias;

    VecN lambda = MatMN::SolveGaussSeidel(A, vb);
    cachedLambda += lambda;

    VecN impulse = Jt * lambda;
    a->ApplyInpulseLinear(Vec2(impulse[0], impulse[1]));
    a->ApplyInpulseAngular(impulse[2]);
    b->ApplyInpulseLinear(Vec2(impulse[3], impulse[4]));
    b->ApplyInpulseAngular(impulse[5]);
}

void JointConstraint::PostSolve() {}
