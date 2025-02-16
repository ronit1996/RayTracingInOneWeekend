#ifndef  SPHERE_H
#define  SPHERE_H

#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
#include <cmath>
#include <memory>
class Sphere : public Hittable{
    private:
    Point3 center;
    double radius;
    shared_ptr<Material> mat;

    public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat) : center(center), radius(std::fmax(0,radius)), mat(mat) {};
    bool hit(const Ray& r, Interval ray_t,  Hit_record& rec) const override{
        auto oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;
        auto discriminant = h*h - a*c;
        // Check if discriminant is less than 0, if yes then the ray doesn't hit anything //
        if(discriminant < 0){
            return false;
        }

        // If it's greater than 0, we have two solutions, check if the smaller root is within ray min and max, if yes, then it hits a sphere //
        auto sqrt = std::sqrt(discriminant);
        auto root1 = (h-sqrt) / a;
        if(root1 >= ray_t.min && root1 <= ray_t.max){
            rec.t = root1;
            rec.P = r.at(root1);
            Vec3 outward_normal = (rec.P - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

        // If the smaller root isn't within min or max, check for the bigger root //
        auto root2 = (h+sqrt) / a;
        if (root2 >= ray_t.min && root2 <= ray_t.max) {
            rec.t = root2;
            rec.P = r.at(root2);
            Vec3 outward_normal = (rec.P - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;
            return true;
        }

        // If both the smaller and bigger roots are outside the min-max bounds //
        return false;
    }

};

#endif
