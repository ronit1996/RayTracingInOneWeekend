#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include <iostream>
#include <cmath>
#include <ostream>

class Vec3{
    public:
    double e[3];

    // Define constructors //
    Vec3(){
        e[0] = 0;
        e[1] = 0;
        e[2] = 0;
    }
    Vec3(double e0, double e1, double e2){
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    // Create operator functions //
    double x() const{
        return e[0];
    }

    double y() const{
        return e[1];
    }

    double z() const{
        return e[2];
    }

    Vec3 operator-() const{
        return Vec3(-e[0], -e[1], -e[2]);
    }

    double operator[](int i) const{
        return e[i];
    }

    double& operator[](int i){
        return e[i];
    }

    Vec3& operator+=(const Vec3& v){
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];

        return *this;
    }

    Vec3& operator*=(double x){
        e[0] *= x;
        e[1] *= x;
        e[2] *= x;

        return *this;
    }

    Vec3& operator/=(double t){
        return *this *= 1/t;
    }

    double length() const{
        return std::sqrt(length_squared());
    }

    double length_squared() const{
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static Vec3 random(){
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(double min, double max){
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// Creating alias of Vec3 named Point3 //
using Point3 = Vec3;

// Creating utility functions //
inline std::ostream& operator<<(std::ostream& out, const Vec3& v){
    return out << v[0] << "," << v[1] << "," << v[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v){
    return Vec3(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v){
    return Vec3(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v){
    return Vec3(u[0]*v[0], u[1]*v[1], u[2]*v[2]);
}

inline Vec3 operator*(double t, const Vec3& v){
    return Vec3(v[0]*t, v[1]*t, v[2]*t);
}

inline Vec3 operator*(const Vec3& v, double t){
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t){
    return (1/t) * v;
}

inline double dot(const Vec3& u, const Vec3& v){
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline Vec3 unit_vector(const Vec3& v){
    return v / v.length();
}

inline Vec3 random_unit_vector() {
    while (true) {
        auto p = Vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = random_unit_vector();
    double dot_product = dot(on_unit_sphere, normal);
    if (dot_product > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline Vec3 random_in_unit_disk(){
    while(true){
        auto p = Vec3(random_double(-1,1), random_double(-1,1),0);
        if(p.length_squared() < 1){
            return p;
        }
    }
}

inline Vec3 reflect(const Vec3& v, const Vec3& n){
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat){
    //calculate r perpendicular//
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);

    //calculate r parallel//
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared()))*n;

    return r_out_perp + r_out_parallel;
}

#endif
