#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vector3
{

    public:

        double e[3];

    public:

        Vector3() : e{0,0,0} {}
        Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        Vector3 operator-() const { return Vector3{-e[0], -e[1], -e[2]}; }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        Vector3& operator+=(const Vector3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vector3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vector3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static Vector3 random() {
            return {random_double(), random_double(), random_double()};
        }

        inline static Vector3 random(double min, double max) {
            return {random_double(min,max), random_double(min,max), random_double(min,max)};
        }

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

};

using Color = Vector3;
using Point3 = Vector3;

#endif