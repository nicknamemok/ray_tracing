#ifndef VECTOR3_UTILITY_H
#define VECTOR3_UTILITY_H

#include "common.h"

inline std::ostream& operator<<(std::ostream &out, const Point3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline std::ostream& write_color(std::ostream &out, const Color &pixel_color, int samples_per_pixel) {

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';

}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v) {
    return {u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
}

inline Vector3 operator*(double t, const Vector3 &v) {
    return {t*v.x(), t*v.y(), t*v.z()};
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return {t * v};
}

inline Vector3 operator/(Vector3 v, double t) {
    return {(1/t) * v};
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return {u.y() * v.z() - u.z() * v.y(),
            u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() * v.x()};
}

inline Vector3 unit_vector(Vector3 v) {
    return {v / v.length()};
}

Vector3 random_in_unit_sphere() {
    while (true) {
        auto p{Vector3::random(-1,1)};
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vector3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

Vector3 random_in_hemisphere(const Vector3& normal) {
    Vector3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2*dot(v,n)*n;
}

Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

Vector3 random_in_unit_disk() {
    while (true) {
        auto p = Vector3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

#endif