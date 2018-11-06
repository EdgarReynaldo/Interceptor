



#include "Circle.hpp"
#include "Vector2.hpp"

#include <cmath>




bool MakeCirclesStick(Circle* c1 , Circle* c2) {
   if (!c1 || !c2) {return false;}
   c1->Stop();
   c2->Stop();
   return true;
}

bool MakeCirclesReverse(Circle* c1 , Circle* c2) {
   if (!c1 || !c2) {return false;}
   c1->Reverse();
   c2->Reverse();
   return true;
}


bool MakeCirclesBounce(Circle* c1 , Circle* c2) {
   if (!c1 || !c2) {return false;}
   
   /// Normalized normal vectors, these point from one circle towards the other
   Vec2 N1 = Vec2(c2->cx - c1->cx , c2->cy - c1->cy).Normalize();
   Vec2 N2 = Vec2(c1->cx - c2->cx , c1->cy - c2->cy).Normalize();
   
   Vec2 V1(c1->vx , c1->vy);
   Vec2 V2(c2->vx , c2->vy);
   
   Vec2 I1 = V1*c1->mass;
   Vec2 I2 = V2*c2->mass;
   Vec2 Itotal = I1 + I2;
   
   /// The angle between V and N determines how much energy is transferred
   
   
   
}


CRESOLVER cresolver = MakeCirclesReverse;



Circle::Circle() :
      cx(0.0),
      cy(0.0),
      rad(0),
      vx(0.0),
      vy(0.0),
      active(false),
      fixed(false),
      density(1.0),
      mass(0.0)
{}



Circle::Circle(double x , double y , double r) :
      cx(x),
      cy(y),
      rad(r),
      vx(0.0),
      vy(0.0),
      active(true),
      fixed(false),
      density(1.0),
      mass(0.0)
{
   mass = M_PI*rad*rad;
}



bool Overlaps(const Circle& c1 , const Circle& c2) {
   const double rsq = c1.rad*c1.rad + c2.rad*c2.rad;
   const double dx = c2.cx - c1.cx;
   const double dy = c2.cy - c1.cy;
   return rsq >= dx*dx + dy*dy;
}



double CalculateCollision(Circle* c1 , Circle* c2) {
   if (!c1 || !c2) {return -1.0;}
   
   const double dx = c2->cx - c1->cx;
   const double dy = c2->cy - c1->cy;
   const double dvx = c2->vx - c1->vx;
   const double dvy = c2->vy - c1->vy;
   const double rsq = (c1->rad + c2->rad)*(c1->rad + c2->rad);
   
   /// Quadratic equation Ax^2 + Bx + C = 0
   const double A = (dvx*dvx + dvy*dvy);
   const double B = 2.0*(dvx*dx + dvy*dy);
   const double C = dx*dx + dy*dy - rsq;

   if (A == 0.0) {
      return -1.0;/// No relative movement, collision is impossible
   }
   
   /// Overlap check here - if they are already colliding at time 0.0, let them pass undisturbed
   if (C < 0.0) {return -1.0;}

   const double TWOA = 2.0*A;
   const double DISCRIM = B*B - 4.0*A*C;
   if (DISCRIM < 0.0) {return -1.0;}/// no real roots
   else if (DISCRIM == 0.0) {
      return -B/TWOA;
   }
   const double SQRTD = sqrt(DISCRIM);
   const double T1 = (-B - SQRTD)/TWOA;
   const double T2 = (-B + SQRTD)/TWOA;
   if (T1 >= 0.0) {
      return T1;
   }
   else if (T2 >= 0.0) {
      return T2;
   }
   return -1.0;/// No collision possible in the future
}




