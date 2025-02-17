#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
#include <cmath>

class Material{
    public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray_in, const Hit_record& rec, Color3& attenuation, Ray& scattered) const{
        return false;
    }
};

class Lambertian : public Material{
    private:
    Color3 albedo;

    public:
    Lambertian(const Color3& albedo) : albedo(albedo) {};

    bool scatter(const Ray& ray_in, const Hit_record& rec, Color3& attenuation, Ray& scattered)const override {
        auto scatter_direction = rec.N + random_unit_vector();
        if(scatter_direction.near_zero()){
            scatter_direction = rec.N;
        }
        scattered = Ray(rec.P, scatter_direction);
        attenuation = albedo;
        return true;
    }

};

class Metal : public Material{
    private:
    Color3 albedo;
    double fuzz;
    public:
    Metal(const Color3& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {};

    bool scatter(const Ray& ray_in, const Hit_record& rec, Color3& attenuation, Ray& scattered)const override{
        Vec3 reflected = reflect(ray_in.direction(), rec.N);
        reflected = reflected + (fuzz * random_unit_vector());
        scattered = Ray(rec.P+(rec.N*0.001), reflected);
        attenuation = albedo;
        //return (dot(scattered.direction(), rec.N) > 0);
        return true;
    }
};

class Dielectric : public Material{
    private:
    double refraction_index;
    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance //
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
    public:
    Dielectric(double refraction_index) : refraction_index(refraction_index) {};

    bool scatter(const Ray& ray_in, const Hit_record& rec, Color3& attenuation, Ray& scattered)const override{
        attenuation = Color3(1.0, 1.0, 1.0);
        double ri;
        if(rec.front_face){
            ri = 1.0/refraction_index;
        }else{
            ri = refraction_index;
        }

        Vec3 unit_direction = unit_vector(ray_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.N), 1);
        double sin_theta = std::sqrt(1 - cos_theta*cos_theta);
        Vec3 direction;
        if(ri * sin_theta > 1 || reflectance(cos_theta, ri) > random_double()){
            direction = reflect(unit_direction, rec.N);
        }else{
            direction = refract(unit_direction, rec.N, ri);
        }
        scattered = Ray(rec.P, direction);
        return true;
    }
};

#endif
