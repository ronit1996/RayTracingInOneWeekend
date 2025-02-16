#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <ostream>
#include "vec3.h"
#include "interval.h"

using Color3 = Vec3;

// Gamma correction //
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ostream &out, const Color3 &pixel_color){
    auto r = linear_to_gamma(pixel_color[0]);
    auto g = linear_to_gamma(pixel_color[1]);
    auto b = linear_to_gamma(pixel_color[2]);

    // Translate the 0-1 component values to 0-255 range //
    static const Interval intensity(0.000, 0.999);
    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

    // Write out the color values //
    out<< rbyte <<" "<< gbyte <<" "<< bbyte <<"\n";
}

#endif
