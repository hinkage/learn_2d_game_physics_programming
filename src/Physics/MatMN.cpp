#include "MatMN.h"

MatMN::MatMN() : M(0), N(0), rows(nullptr) {}

MatMN::MatMN(int M, int N) : M(M), N(N) {
    rows = new VecN[M];
    for (int i = 0; i < M; i++) {
        rows[i] = VecN(N);
    }
}

MatMN::MatMN(const MatMN &m) {
    M = m.M;
    N = m.N;
    rows = new VecN[M];
    for (int i = 0; i < M; i++) {
        rows[i] = m.rows[i];
    }
}

MatMN &MatMN::operator=(const MatMN &m) {
    if (this == &m) {
        return *this; // Handle self-assignment
    }
    delete[] rows;
    M = m.M;
    N = m.N;
    rows = new VecN[M];
    for (int i = 0; i < M; i++) {
        rows[i] = m.rows[i];
    }
    return *this;
}

MatMN::~MatMN() { delete[] rows; }

void MatMN::Zero() {
    for (int i = 0; i < M; i++) {
        rows[i].Zero();
    }
}

MatMN MatMN::Transpose() const {
    MatMN result(N, M);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            result.rows[j][i] = rows[i][j];
        }
    }
    return result;
}

VecN MatMN::SolveGaussSeidel(const MatMN &A, const VecN &b) {
    const int N = b.N;
    VecN x(N);
    x.Zero();
    for (int iteration = 0; iteration < N; iteration++) {
        for (int i = 0; i < N; i++) {
            float Aii = A.rows[i][i];
            float dx = (b[i] - A.rows[i].Dot(x)) / Aii;
            // if dx is NaN, this will be false
            if (dx == dx) {
                x[i] += dx;
            }
        }
    }
    return x;
}

VecN MatMN::operator*(const VecN &v) const {
    if (v.N != N) {
        return v;
    }
    VecN result(M);
    for (int i = 0; i < M; i++) {
        result[i] = v.Dot(rows[i]);
    }
    return result;
}

MatMN MatMN::operator*(const MatMN &m) const {
    if (m.M != N && m.N != M) {
        return m;
    }
    MatMN tranposed = m.Transpose();
    MatMN result(M, m.N);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < m.N; j++) {
            result.rows[i][j] = rows[i].Dot(tranposed.rows[j]);
        }
    }
    return result;
}
