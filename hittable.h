#ifndef  HITTABLE_H
#define  HITTABLE_H

#include "interval.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
class Material;
class Hit_record{
    public:
    Point3 P;
    Vec3 N;
    double t;
    bool front_face;
    shared_ptr<Material> mat;

    void set_face_normal(const Ray& r, const Vec3& outward_normal){
        if(dot(r.direction(), outward_normal) > 0){
            front_face = false;
            N = -outward_normal;
        }else{
            front_face = true;
            N = outward_normal;
        }
    }
};

class Hittable{
    public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& r, Interval ray_t, Hit_record& rec) const = 0;
};

#endif
