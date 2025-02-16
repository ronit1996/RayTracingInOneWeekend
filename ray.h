#ifndef  RAY_H
#define  RAY_H
#include "vec3.h"

class Ray{
  private:
  Point3 orig;
  Vec3 dir;

  public:
  // Define Constructors //
  Ray(){};
  Ray(const Point3& origin, const Vec3& direction){
      orig = origin;
      dir = direction;
  }

  // Define accessors //
  const Point3& origin() const{
      return orig;
  }

  const Vec3& direction() const{
      return dir;
  }

  Point3 at(double t) const{
      return orig + dir*t;
  }

};

#endif
