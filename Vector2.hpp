



#ifndef Vector2_HPP
#define Vector2_HPP



class Vec2 {
protected :
   double x;
   double y;
   
   
   
   
public :
   
   Vec2(double xval , double yval);
   Vec2(double xval , double yval) :
         x(xval),
         y(yval)
   {}
   
   inline void Set(double xval , double yval) {
      x = xval;
      y = yval;
   }
   
   
   Vec2& operator*(double f);
   Vec2& operator*(double f) {
      x *= f;
      y *= f;
      return *this;
   }
   Vec2& operator+(const Vec2& v);
   Vec2& operator+(const Vec2& v) {
      x += v.x;
      y += v.y;
      return *this;
   }
   
   
   
   inline void Normalize() {
      const double mag = sqrt(x*x + y*y);
      x /= mag;
      y /= mag;
   }
};




#endif // Vector2_HPP
