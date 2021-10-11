#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
    sphere() {}
    sphere(point3 c, double r, shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - this->center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    else
    {
        auto sqrtd = sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a;
        if (root<t_min || root > t_max) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || root > t_max) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(root);
        vec3 outward_normal = (rec.p - this->center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = this->mat_ptr;
        return true;
    }
}

#endif