



#include "Vector2.hpp"




Vec2::Vec2(double xval , double yval) :
      x(xval),
      y(yval)
{}



void Vec2::Set(double xval , double yval) {
   x = xval;
   y = yval;
}



Vec2& Vec2::Normalize() {
   const double mag = sqrt(x*x + y*y);
   x /= mag;
   y /= mag;
   return *this;
}



Vec2& Vec2::operator*=(double f) {
   *this = *this*f;
   return *this;
}



Vec2& Vec2::operator+=(const Vec2& v) {
   *this = *this + v;
   return *this;
}






