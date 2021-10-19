#ifndef CUBE_H
#define CUBE_H

#include "hittable.h"
#include "vec3.h"
#include<vector>

using std::vector;

class cube : public hittable
{
public:
    cube() {}
    cube(point3 c, double h_length, shared_ptr<material> m) : center(c), half_side_length(h_length), mat_ptr(m) {};

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    void calculate_intersection(vector<struct intersection> &intersection_list, const ray &r, int axis) const;
public:
    point3 center;
    double half_side_length;
    shared_ptr<material> mat_ptr;
};

struct intersection{
    double possible_t;
    int plain_order;
};

void cube::calculate_intersection(vector<struct intersection> &intersection_list, const ray &r, int axis) const {
    if(!r.is_parallel(axis)) {
        double value1, value2, value3, value4;
        double possible_t1 = r.other2axis(center[axis] + half_side_length, axis, &value1, &value2);
        double possible_t2 = r.other2axis(center[axis] - half_side_length, axis, &value3, &value4);
        /*
        corresponding relations between order of axis and order of plains:
        because axis range: 0(x), 1(y), 2(z)
        plain range: 0(x+), 1(x-), 2(y+), 3(y-), 4(z+), 5(z-)
        so the relation is 2n for +, 2n+1 for -:
        0 -> 0, 1
        1 -> 2, 3
        2 -> 4, 5
        */
        
        /*
        because other2axis only returns two non-order values but i 
        need to process different situations according to the axis so i 
        add a switch statement
        */
        switch (axis) {
            case 0:{
                /*
                    value -> y, value2 -> z
                */
                if (value1 < center[1] + half_side_length && value1 > center[1] - half_side_length && value2 < center[2] + half_side_length && value2 > center[2] - half_side_length) { 
                   struct intersection t1;
                    t1.possible_t = possible_t1;
                    t1.plain_order = 2 * axis;
                    intersection_list.push_back(t1);
                }
                if (value3 < center[1] + half_side_length && value3 > center[1] - half_side_length && value4 < center[2] + half_side_length && value4 > center[2] - half_side_length) {
                    struct intersection t2;
                    t2.possible_t = possible_t2;
                    t2.plain_order = 2 * axis + 1;
                    intersection_list.push_back(t2);
                }
                break;
            }
            case 1:{
                /*
                    value1 -> x, value2 -> z
                */
                if (value1 < center[0] + half_side_length && value1 > center[0] - half_side_length && value2 < center[2] + half_side_length && value2 > center[2] - half_side_length) { 
                   struct intersection t1;
                    t1.possible_t = possible_t1;
                    t1.plain_order = 2 * axis;
                    intersection_list.push_back(t1);
                }
                if (value3 < center[0] + half_side_length && value3 > center[0] - half_side_length && value4 < center[2] + half_side_length && value4 > center[2] - half_side_length) {
                    struct intersection t2;
                    t2.possible_t = possible_t2;
                    t2.plain_order = 2 * axis + 1;
                    intersection_list.push_back(t2);
                }
                break;
            }
            case 2:{
                /*
                    value1 -> x, value2 -> y
                */
                if (value1 < center[0] + half_side_length && value1 > center[0] - half_side_length && value2 < center[1] + half_side_length && value2 > center[1] - half_side_length) { 
                   struct intersection t1;
                    t1.possible_t = possible_t1;
                    t1.plain_order = 2 * axis;
                    intersection_list.push_back(t1);
                }
                if (value3 < center[0] + half_side_length && value3 > center[0] - half_side_length && value4 < center[1] + half_side_length && value4 > center[1] - half_side_length) {
                    struct intersection t2;
                    t2.possible_t = possible_t2;
                    t2.plain_order = 2 * axis + 1;
                    intersection_list.push_back(t2);
                }
                break;
            }
            default:{
                /*
                    will not go to this branch
                */
                break;
            }
        }
    }
}


bool cube::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    /*  how to express cube surface:
        x = +- a, -a <= y <= a, -a <= z <= a
        y = +- a, -a <= x <= a, -a <= z <= a
        z = +- a, -a <= x <= a, -a <= y <= a

        order the plains in 0-5:
        x+, x-, y+, y-, z+, z-: 0, 1, 2, 3, 4, 5
    */

    vector<struct intersection> intersection_list;

    for (int i =0; i<= 2; ++i) {
        calculate_intersection(intersection_list, r, i);
    }

    int root_num = intersection_list.size();
    struct intersection root1, root2;
    switch (root_num)
    {
    case 0:
        return false;
    case 1:
        root1 = intersection_list[0];
        if (root1.possible_t < t_min || root1.possible_t > t_max) {
            return false;
        }
    case 2:
        root1 = intersection_list[0];
        root2 = intersection_list[1];
        if (root1.possible_t > root2.possible_t) {
            struct intersection tmp_root = root2;
            root2 = root1;
            root1 = tmp_root;
        }
        if (root1.possible_t < t_min || root1.possible_t > t_max) {
            root1 = root2;
            if (root1.possible_t < t_min || root1.possible_t > t_max) {
                return false;
            }
        }
    default:
        break;
    }

    rec.t = root1.possible_t;
    rec.p = r.at(root1.possible_t);
    rec.mat_ptr = this->mat_ptr;
    switch(root1.plain_order) {
        case 0: {
            rec.set_face_normal(r, vec3(1, 0, 0));
            break;
        }
        case 1: {
            rec.set_face_normal(r, vec3(-1, 0, 0));
            break;
        }
        case 2: {
            rec.set_face_normal(r, vec3(0, 1, 0));
            break;
        }
        case 3: {
            rec.set_face_normal(r, vec3(0, -1, 0));
            break;
        }
        case 4: {
            rec.set_face_normal(r, vec3(0, 0, 1));
            break;
        }
        case 5: {
            rec.set_face_normal(r, vec3(0, 0, -1));
            break;
        }
        default:
            std::cout<<"flow beyound control\n";
    }
    return true;

};

#endif