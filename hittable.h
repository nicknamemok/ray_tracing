#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "material.h"

class material;

class hittable {

    public:

        Point3 p;
        Vector3 normal;
        double t;
        bool front_face;
        shared_ptr<material> mat_ptr;

    public:

        virtual bool hit(const Ray& r, double t_min, double t_max) = 0;

        inline void set_face_normal(const Ray& r, const Vector3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal :-outward_normal;
        }

};

#endif