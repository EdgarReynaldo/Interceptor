



#ifndef Vector2_HPP
#define Vector2_HPP

#include <cmath>

class Vec2 {
public :
   double x;
   double y;

   Vec2(double xval , double yval);
   
   void Set(double xval , double yval);
   Vec2& Normalize();
   
   Vec2& operator*=(double f);
   Vec2& operator+=(const Vec2& v);
   
   inline Vec2 operator-() {return Vec2(-x,-y);}
   
///   double Magnitude();
   inline double Magnitude() {
      return sqrt(x*x + y*y);
   }

};


inline Vec2 operator+(const Vec2& v1 , const Vec2& v2) {
   return Vec2(v1.x + v2.x , v1.y + v2.y);
}
inline Vec2 operator-(const Vec2& v1 , const Vec2& v2) {
   return Vec2(v1.x - v2.x , v1.y - v2.y);
}


inline Vec2 operator*(const Vec2& v , double f) {
   return Vec2(v.x*f , v.y*f);
}

///double DotProduct(const Vec2& v1 , const Vec2& v2);
inline double DotProduct(const Vec2& v1 , const Vec2& v2) {
   return v1.x*v2.y + v2.x*v1.y;
}

#endif // Vector2_HPP
