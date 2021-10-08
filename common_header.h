#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include<cmath>
#include<memory>
#include<limits>
#include<cstdlib>

#include "ray.h"
#include "vec3.h"

using std::shared_ptr;
using std::make_shared;

/* I think it's unnessesary to use 'using sqrt' 
because in header "vec3.h", it actually has already used 'sqrt'.
Later debug this by commenting the line below to see if there will
arouse any problems.
 */

using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree2radian (const double degree) {
    return degree * pi / 180.0;
}

// inline double random_double() {
//     // [0, 1)
//     return rand() / (RAND_MAX +1.0);
// }

// inline double random_double(double min, double max) {
//     // [min, max)
//     return (max - min) * random_double();
// }

inline double clamp(double x, double min, double max) {
    if (x < min)
        return min;
    if (x > max) 
        return max;
    return x;
}

#endif