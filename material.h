#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"

class hittable;

class material {
    public:
        virtual bool scatter(
            const Ray& r_in, Color& attenuation, Ray& scattered, const Point3& p, const Vector3& normal, const bool front_face
        ) const = 0;
};

class lambertian : public material {

    public:

        Color albedo;

    public:

        lambertian(const Color& a) : albedo(a) {}

        virtual bool scatter(
            const Ray& r_in, Color& attenuation, Ray& scattered, const Point3& p, const Vector3& normal, const bool front_face
        ) const override {
            auto scatter_direction = normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = normal;

            scattered = Ray{p, scatter_direction};
            attenuation = albedo;
            return true;
        }

};

class metal : public material {

    public:

        Color albedo;
        double fuzz;

    public:
        metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const Ray& r_in, Color& attenuation, Ray& scattered, const Point3& p, const Vector3& normal, const bool front_face
        ) const override {
            Vector3 reflected = reflect(unit_vector(r_in.direction()), normal);
            scattered = Ray{p, reflected + fuzz*random_in_unit_sphere()};
            attenuation = albedo;
            return (dot(scattered.direction(), normal) > 0);
        }


};

class dielectric : public material {

    public:

        double ir; // Index of Refraction

    public:
        dielectric(double index_of_refraction) : ir{index_of_refraction} {}

        virtual bool scatter(
            const Ray& r_in, Color& attenuation, Ray& scattered, const Point3& p, const Vector3& normal, const bool front_face
        ) const override {

            attenuation = Color{1.0, 1.0, 1.0};
            double refraction_ratio = front_face ? (1.0/ir) : ir;

            Vector3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vector3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, normal);
            else
                direction = refract(unit_direction, normal, refraction_ratio);

            scattered = Ray(p, direction);
            return true;
        }

    private:
    
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif