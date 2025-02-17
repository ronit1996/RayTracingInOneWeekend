#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
#include "color.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include "material.h"

class Camera{
    private:
    int image_height;
    Point3 camera_center; //camera center//
    Point3 pixel00_loc; //location of the first pixel//
    Vec3 pixel_delta_u; //offset pixel to the right//
    Vec3 pixel_delta_v; //offset piexel to down //
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples//
    Vec3 u, v, w; //camera frame basis vectors//
    Vec3 defocus_disk_u; //defocus disk/lens horizontal radius//
    Vec3 defocus_disk_v; //defocus disk/lens vertical radius//

    void initialize(){
        image_height = int(image_width/aspect_ratio);
        if(image_height < 1){
            image_height = 1;
        }else{
            image_height *= 1;
        }

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center = lookfrom;

        // Create viewport //
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the u, v, w vectors //
        w = unit_vector(lookfrom-lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors of horizontal and vertical viewport edges //
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the pixel width and height //
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel //
        auto viewport_upper_left = camera_center - (focus_dist * w) - (viewport_u/2) - (viewport_v/2);
        pixel00_loc = viewport_upper_left + (0.5 * (pixel_delta_u+pixel_delta_v));

        // Calculate the camera defocus disk basis vectors //
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;

    }

    Ray get_ray(int i, int j) const {
            // Construct a camera ray originating from the defocus disk and directed at randomly sampled
            // point around the pixel location i, j.

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                              + ((i + offset[0]) * pixel_delta_u)
                              + ((j + offset[1]) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return Ray(ray_origin, ray_direction);
        }

        Vec3 sample_square() const {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        Point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

    /*Color3 ray_color(const Ray& r, const Hittable& world, int depth){
        if (depth <= 0) {
                return Vec3(0, 0, 0);  // Stop recursion, return black
            }

        Hit_record rec;

        if (world.hit(r, Interval(0.001, infinity), rec)) {
            Ray scattered;
            Color3 attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, world, depth-1);
        }else{
            Vec3 unit_direction = unit_vector(r.direction());
                auto a = 0.5*(unit_direction[1] + 1.0);
                return (1.0-a)*Color3(1.0, 1.0, 1.0) + a*Color3(0.1, 0.3, 1.0);
        }
    }*/

    Color3 ray_color(Ray r, const Hittable& world, int depth){
        Color3 col(1.0,1.0,1.0);
        Hit_record rec;
        Color3 attenuation;
        Ray scattered;
        for(int bounce=0; bounce<depth; bounce++){
            if (world.hit(r, Interval(0.001, infinity), rec)){
                if(rec.mat->scatter(r, rec, attenuation, scattered)){
                    r = scattered;
                    col = col * attenuation;
                }else{
                    break;
                }
            }else{
                break;
            }
        }
        //Background color//
        Vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction[1] + 1.0);
        return col * ((1.0-a)*Color3(1.0, 1.0, 1.0) + a*Color3(0.1, 0.3, 1.0));
    }

    public:
    // Image //
    int image_width;
    double aspect_ratio;
    int  samples_per_pixel = 10; // Count of random samples for each pixel //
    double vfov = 90; //vertical view angle//
    Vec3 lookfrom = Point3(0,0,0); //the position of the camera//
    Vec3 lookat = Point3(0,0,-1); //where the camera is looking at//
    Vec3 vup = Vec3(0,1,0); //up vector//
    double defocus_angle = 0; //variation angle of rays through each pixel//
    double focus_dist = 10; //distance between camera lookfrom (center) to the focal plane//

    void render(const Hittable& world){
        // Start time //
        auto start = std::chrono::high_resolution_clock::now();

        initialize();

        std::cout<<"P3"<<"\n"<<image_width<<" "<<image_height<<"\n"<<"255"<<"\n";
        for(int j=0; j<image_height; j++){
            std::clog << "\rRendering done: " << int((j*1.0/image_height)*100) << "%" << ' ' << std::flush;
            for(int i=0; i<image_width; i++){
                Color3 pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world, 100);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        // Stop time and measure //
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        long long ms = duration.count();

        // Convert to hours, minutes, and seconds
            long long hours = ms / (1000 * 60 * 60);
            ms %= (1000 * 60 * 60);
            long long minutes = ms / (1000 * 60);
            ms %= (1000 * 60);
            long long seconds = ms / 1000;
            ms %= 1000;

            std::clog <<"\n"<< "Execution time: "
                      << hours << "h "
                      << minutes << "m "
                      << seconds << "s "
                      << ms << "ms"
                      << "\n";

        std::clog << "\rDone.                         \n";
    }

};
#endif
