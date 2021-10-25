#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <fstream>
#include "common.h"

#include "vector3.h"
#include "vector3_utility.h"
#include "ray.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"


class Scene
{

    public:

        Camera cam;

        const int image_width;
        const int image_height;
        const int samples_per_pixel = 100;
        const int max_depth = 50;
        const bool alternative_diffuse_formulation;

        hittable_list world;
        std::ofstream file;

    public:

        Scene(Camera c, hittable_list w, int width, int height, bool alternative=false)
            : cam{c}, image_width{width} , image_height{height}
            , alternative_diffuse_formulation{alternative}, world{w}
        {
            file.open("test.ppm");
        }

        Vector3 ray_color(const Ray& r, hittable& world, int depth)
        {

            // If we've exceeded the ray bounce limit, no more light is gathered.
            if (depth <= 0)
                return Color{0,0,0};

            // Check if this ray collides with any of the objects in world (i.e. hittable list)
            if (world.hit(r, 0.001, infinity)) {
                Ray scattered;
                Color attenuation;
                Point3 p = world.p;
                Vector3 normal = world.normal;
                bool front_face = world.front_face;
                if (world.mat_ptr->scatter(r, attenuation, scattered, p, normal, front_face))
                    return attenuation * ray_color(scattered, world, depth-1);
                return Color{0,0,0};
            }

            // If not, default to "background image"
            Vector3 unit_direction = unit_vector(r.direction());
            auto t = 0.5*(unit_direction.y() + 1.0);
            return (1.0-t)*Color{1.0, 1.0, 1.0} + t*Color{0.5, 0.7, 1.0};

        }

        // Render
        void render()
        {
            file << "P3\n" << image_width << " " << image_height << "\n255\n";

            for (int j = image_height-1; j >= 0; --j) {

                std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

                for (int i = 0; i < image_width; ++i) {
                    Color pixel_color(0, 0, 0);

                    for (int s = 0; s < samples_per_pixel; ++s) {

                        auto u = (i + random_double()) / (image_width-1);
                        auto v = (j + random_double()) / (image_height-1);
                        Ray r = cam.get_ray(u, v);
                        pixel_color += ray_color(r, world, max_depth);

                    }
                    write_color(file, pixel_color, samples_per_pixel);
                }

            }

            file.close();
            std::cerr << "\nDone.\n";
        }

};


#endif