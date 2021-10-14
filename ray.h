#ifndef RAY_H
#define RAY_H

#include "vec3.h"


class ray
{
public:
    ray() {}
    ray(const point3 &origin, const vec3 &direction)
        : orig(origin), dir(direction)
    {
    }

    point3 origin() const
    {
        return orig;
    }
    vec3 direction() const
    {
        return dir;
    }
    point3 at(double t) const
    {
        return orig + t * dir;
    }

    bool is_parallel(int position) const {
        if (dir.e[position] == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    
    double other2axis(double x, int position, double *value1, double *value2) const
    {
        double t = (x - orig.e[position]) / dir.e[position];
        point3 intersection = at(t);
        switch(position) {
            case 0: {
                *value1 = intersection.y();
                *value2 = intersection.z();
                break;
            }
            case 1: {
                *value1 = intersection.x();
                *value2 = intersection.z();
                break;
            }
            case 2: {
                *value1 = intersection.x();
                *value2 = intersection.y();
                break;
            }
        }
        return t;
    }

public:
    point3 orig;
    vec3 dir;
};
#endif