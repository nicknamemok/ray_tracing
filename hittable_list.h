#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {

    public:

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const Ray& r, double t_min, double t_max) override;

    public:

        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const Ray& r, double t_min, double t_max) {

    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {

        if (object->hit(r, t_min, closest_so_far)) {
            
            hit_anything = true;
            closest_so_far = object->t;

            // If hit, we update the t,p,normal,and front_face values for hittable_list of that specific ray
            t = object->t;
            p = object->p;
            normal = object->normal;
            front_face = object->front_face;
            mat_ptr = object->mat_ptr;

        }
    }

    return hit_anything;
}

#endif