#pragma once

struct VecN {
    int N;
    float *data;

    VecN();
    VecN(int N);
    VecN(const VecN &v);
    ~VecN();

    void Zero();
    float Dot(const VecN &v) const;

    VecN& operator=(const VecN &v);
    const VecN operator+(const VecN &v);
    const VecN operator-(const VecN &v);
    const VecN operator*(const float n);

    VecN &operator+=(const VecN &v);
    VecN &operator-=(const VecN &v);
    VecN &operator*=(const float n);

    float operator[](const int index) const;
    float &operator[](const int index);
};
