#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include<cmath>
#include<memory>
#include<limits>

#include "ray.h"
#include "vec3.h"

using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree2radian (const double degree) {
    return degree * pi / 180.0;
}

#endif