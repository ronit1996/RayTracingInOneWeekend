#ifndef INTERVAL_H
#define INTERVAL_H
#include "rtweekend.h"

class Interval{
    public:
    double min, max;
    Interval() : min(+infinity), max(-infinity){};
    Interval(double min, double max) : min(min), max(max) {};

    double size()const{
        return max - min;
    }

    bool contains(double x)const{
        return min <= x && x <= max;
    }

    bool surround(double x)const{
        return min < x && x < max;
    }

    double clamp(double x) const {
        if(x < min){
            return min;
        }else if(x > max){
            return max;
        }else{
            return x;
        }
    }

    static const Interval empty, universe;
};

const Interval Interval::empty    = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif
