#ifndef CAMERA_H
#define CAMERA_H

#include "common_header.h"

class camera {
public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov /* in degrees */, double aspect_ratio, double aperture, double focus_disk) {
        auto theta = degree2radian(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_disk * viewport_width * u;
        vertical = focus_disk * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_disk * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const{
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 w, u, v;
    double lens_radius;
};


#endif