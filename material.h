#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

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

#endif
