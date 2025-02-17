#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <cmath>
#include "material.h"
#include "vec3.h"

int main(){
    // Create the scene world //
    Hittable_list world;
    auto ground_material = make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.18, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.7) {
                    // diffuse
                    auto albedo = Color3::random() * Color3::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    /*auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));
    */
    auto material2 = make_shared<Lambertian>(Color3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    // Create camera object //
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
        cam.image_width = 1920;
        cam.samples_per_pixel = 25;

        cam.vfov     = 7;
        cam.lookfrom = Point3(20,1,3);
        cam.lookat   = Point3(0,0,0);
        cam.vup      = Vec3(0,1,0);

        cam.defocus_angle = 0.6;
        cam.focus_dist    = 15.0;
    cam.render(world);
}
