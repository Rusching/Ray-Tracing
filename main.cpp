#include "common_header.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "cube.h"

#include <iostream>
#include "color.h"
#include <iostream>
#include "material.h"
color ray_color(const ray &r, const hittable& world, int depth) 
{
    // Set the condition to stop recurcy 
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)){
        point3 target = rec.p + random_in_hemisphere(rec.normal);
        // Note it introduces recurcy here which may arouse stack overflow
        // Origin implemention use rgb to represent the color, but now just use the gray color so 
        // that the image will appear to be dim

        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth -1);
        return color(0, 0, 0);
        // return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
        // return (0.5 * (rec.normal + vec3(1, 1, 1)));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
};

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -3; a< 3; a++) {
        for (int b = -3; b < 3; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse, 80%
                    auto aldebo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(aldebo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // Metal, 15%
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // Glass, 5%
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list cube_test() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto material = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<cube>(point3(0, 1, 0), 0.5, material));
    return world;
}

int main()
{

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 150;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World

    // Here add two spheres, and one very big to be considered as ground
    auto R = cos(pi / 4);
    hittable_list world = cube_test();

    // Camera

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    // auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s< samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width -1);
                auto v = (j + random_double()) / (image_height -1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
        std::cout << '\n';
    }
    std::cerr << "\nDone.\n";
}