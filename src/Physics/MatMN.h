#pragma once

#include "VecN.h"

struct MatMN {
    int M;
    int N;
    VecN *rows;

    MatMN();
    MatMN(int M, int N);
    MatMN(const MatMN &m);
    ~MatMN();

    void Zero();
    MatMN Transpose() const;
    static VecN SolveGaussSeidel(const MatMN& A, const VecN& b);

    MatMN &operator=(const MatMN &m);
    VecN operator*(const VecN &v) const;
    MatMN operator*(const MatMN &m) const;
};
