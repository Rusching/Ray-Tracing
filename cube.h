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

    void calculate_intersection(vector<double> &intersection_list, const ray &r, int axis) const;
public:
    point3 center;
    double half_side_length;
    shared_ptr<material> mat_ptr;
};

void cube::calculate_intersection(vector<double> &intersection_list, const ray &r, int axis) const {
    if(!r.is_parallel(axis)) {
        double value1, value2, value3, value4;
        double possible_t1 = r.other2axis(half_side_length, axis, &value1, &value2);
        double possible_t2 = r.other2axis(-half_side_length, axis, &value3, &value4);
        if (value1 < half_side_length && value1 > -half_side_length && value2 < half_side_length && value2 > -half_side_length) {
            intersection_list.push_back(possible_t1);
        }
        if (value3 < half_side_length && value3 > -half_side_length && value4 < half_side_length && value4 > -half_side_length) {
            intersection_list.push_back(possible_t2);
        }
    }
}


bool cube::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    /*  how to express cube surface:
        x = +- a, -a <= y <= a, -a <= z <= a
        y = +- a, -a <= x <= a, -a <= z <= a
        z = +- a, -a <= x <= a, -a <= y <= a
    */

    vector<double> intersection_list;

    for (int i =0; i<= 2; ++i) {
        calculate_intersection(intersection_list, r, i);
    }

    int root_num = intersection_list.size();
    int root1, root2;
    switch (root_num)
    {
    case 0:
        return false;
    case 1:
        root1 = intersection_list[0];
        if (root1 < t_min || root1 > t_max) {
            return false;
        }
    case 2:
        root1 = intersection_list[0];
        root2 = intersection_list[1];
        if (root1 > root2) {
            int tmp_root = root2;
            root2 = root1;
            root1 = tmp_root;
        }
        if (root1 < t_min || root1 > t_max) {
            root1 = root2;
            if (root1 < t_min || root1 > t_max) {
                return false;
            }
        }
    default:
        break;
    }

    rec.t = root1;
    rec.p = r.at(root1);
    rec.mat_ptr = this->mat_ptr;
    vec3 outward_normal;
    if (rec.p.x() == half_side_length) {
        outward_normal = vec3(1, 0, 0);
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    else if (rec.p.x() == -half_side_length){
        outward_normal = vec3(-1, 0, 0);
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    else if (rec.p.y() == half_side_length) {
        outward_normal = vec3(0, 1, 0);
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    else if (rec.p.y() == -half_side_length) {
        outward_normal = vec3(0, -1, 0);
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    else if (rec.p.z() == half_side_length) {
        outward_normal = vec3(0, 0, 1);
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    else if (rec.p.z() == -half_side_length) {
        outward_normal = vec3(0, 0, -1);
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    std::cout<< "flow beyound control" << "\n";

};

#endif