#include "camera.h"
#include "scene.h"

hittable_list generate_world()
{
    hittable_list w;

    auto ground_material = make_shared<lambertian>(Color{0.5, 0.5, 0.5});
    w.add(make_shared<sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    w.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    w.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    w.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    w.add(make_shared<sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(Color(0.4, 0.2, 0.1));
    w.add(make_shared<sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.0);
    w.add(make_shared<sphere>(Point3(4, 1, 0), 1.0, material3));

    return w;
}

int main() {

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    Vector3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera c1{lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus};
    Scene s1{c1,generate_world(),image_width, image_height};

    s1.render();

}

/*
Inputs: image_width, aspect_ratio, lookfrom, lookat, vup, dist_to_focus, aperture
Keep: lookfrom, look at, vup, dist_to_focus, aperture
Manually input image_width and height?
*/

