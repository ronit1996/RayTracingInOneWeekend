#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <cmath>
#include "material.h"

int main(){
    // Create the scene world //
    Hittable_list world;
    auto material_ground = make_shared<Lambertian>(Color3(0.8, 0.8, 0.0));
        auto material_center = make_shared<Lambertian>(Color3(0.1, 0.2, 0.5));
        auto material_left   = make_shared<Metal>(Color3(0.8, 0.8, 0.8),0.1);
        auto material_right  = make_shared<Metal>(Color3(0.8, 0.6, 0.2),0.3);

        world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
        world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
        world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
        world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Create camera object //
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1920;
    cam.samples_per_pixel = 50;

    cam.render(world);
}
