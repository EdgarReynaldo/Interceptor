



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
   if (x == 0.0 && y == 0.0) {return *this;}
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



Vec2 ScalarProjection(Vec2 A , Vec2 B) {
   B.Normalize();
   return B*DotProduct(A,B);
}



