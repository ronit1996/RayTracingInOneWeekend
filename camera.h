#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include <chrono>
#include <iostream>
#include "material.h"

class Camera{
    private:
    int image_height;
    Point3 camera_center; //camera center//
    Point3 pixel00_loc; //location of the first pixel//
    Vec3 pixel_delta_u; //offset pixel to the right//
    Vec3 pixel_delta_v; //offset piexel to down //
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples

    void initialize(){
        image_height = int(image_width/aspect_ratio);
        if(image_height < 1){
            image_height = 1;
        }else{
            image_height *= 1;
        }

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center = Point3(0,0,0);

        // Create viewport //
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height*(double(image_width)/image_height);
        auto camera_center = Point3(0,0,0);

        // Calculate the vectors of horizontal and vertical viewport edges //
        auto viewport_u = Vec3(viewport_width, 0, 0);
        auto viewport_v = Vec3(0, -viewport_height, 0);

        // Calculate the pixel width and height //
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel //
        auto viewport_upper_left = camera_center - Vec3(0,0,focal_length) - (viewport_u/2) - (viewport_v/2);
        pixel00_loc = viewport_upper_left + (0.5 * (pixel_delta_u+pixel_delta_v));

    }

    Ray get_ray(int i, int j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled
            // point around the pixel location i, j.

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                              + ((i + offset[0]) * pixel_delta_u)
                              + ((j + offset[1]) * pixel_delta_v);

            auto ray_origin = camera_center;
            auto ray_direction = pixel_sample - ray_origin;

            return Ray(ray_origin, ray_direction);
        }

        Vec3 sample_square() const {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
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
    int  samples_per_pixel = 10; // Count of random samples for each pixel

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
