



#include "Circle.hpp"
#include "Vector2.hpp"

#include <cmath>

double ELASTICITY = 1.0;/// No energy lost
//double ELASTICITY = 0.5;/// Half energy lost

double EPSILON = 0.0001;


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
   
   const double m1 = c1->mass;
   const double m2 = c2->mass;

   Vec2 I1 = V1*m1;
   Vec2 I2 = V2*m2;
///   double Itotal = (I1 + I2).Magnitude();
   
   /// The angle between V and N determines how much energy is transferred
   
   const double I1M = I1.Magnitude();
   const double I2M = I2.Magnitude();

   double cosA1 = 0.0;
   double cosA2 = 0.0;
   
   Vec2 I1N(0,0);///N1*cosA1*I1M;/// Momentum of circle one in the normal direction
   Vec2 I2N(0,0);/// = N2*cosA2*I2M;/// Momentum of circle two in the normal direction
   if (I1M > 0.0) {
      I1N = N1*DotProduct(I1 , N1);/// /I1M;/// Magnitude of N is always 1, they're normalized
   }
   if (I2M > 0.0) {/// Object has momentum
///      cosA2 = DotProduct(I2 , N2)/I2M;/// Magnitude of N is always 1, they're normalized
      I2N = N2*DotProduct(I2 , N2);/// /I2M;/// Magnitude of N is always 1, they're normalized
   }
   
///   Vec2 I1N = N1*cosA1*I1M;/// Momentum of circle one in the normal direction
///   Vec2 I2N = N2*cosA2*I2M;/// Momentum of circle two in the normal direction

   if (cosA1 > 0.0) {
      /// Circle one is moving towards circle two
      /// Give circle ones normal momentum to circle two
      I1 = I1 - I1N;/// This momentum is lost, transferred to the other circle
      I1N *= ELASTICITY;/// Energy is lost due to inelasticity
      if (c2->fixed) {/// We hit an immovable object
         I1N *= -1.0;/// Reflection of normal energy
         I1 += I1N;/// rebound effect
      }
      else {
         I2 = I2 + I1N;/// The remaining momentum is gained by the other circle
      }
   }

   if (cosA2 > 0.0) {
      /// Circle two is moving towards circle one
      /// Give circle twos normal momentum to circle one
      I2 = I2 - I2N;/// This momentum is lost
      I2N *= ELASTICITY;/// Energy lost due to inelasticity
      if (c1->fixed) {/// We hit an immovable object
         I2N *= -1.0;/// Reflection of normal energy
         I2 += I2N;/// rebound effect
      }
      else {
         I1 = I1 + I2N;/// The remaining momentum is gained by the other circle
      }
   }
///   double Itotal2 = (I1 + I2).Magnitude();
   
   bool changed = false;
   if (!c1->fixed && m1 > 0.0) {
      V1 = I1*(1.0/m1);
      c1->SetSpeed(V1.x , V1.y);
      changed = true;
   }
   if (!c2->fixed && m2 > 0.0) {
      V2 = I2*(1.0/m2);
      c2->SetSpeed(V2.x , V2.y);
      changed = true;
   }
   return changed;
}



bool MakeCirclesBounce2(Circle* c1 , Circle* c2) {
   if (!c1 || !c2) {return false;}
   
   /// Normalized normal vectors, these point from one circle towards the other
   Vec2 N1 = Vec2(c2->cx - c1->cx , c2->cy - c1->cy).Normalize();
   Vec2 N2 = Vec2(c1->cx - c2->cx , c1->cy - c2->cy).Normalize();
   
   Vec2 V1(c1->vx , c1->vy);
   Vec2 V2(c2->vx , c2->vy);
   
   const double m1 = c1->mass;
   const double m2 = c2->mass;

   Vec2 I1 = V1*m1;
   Vec2 I2 = V2*m2;
///   double Itotal = (I1 + I2).Magnitude();
   
   /// The angle between V and N determines how much energy is transferred
   
   const double I1M = I1.Magnitude();
   const double I2M = I2.Magnitude();

   Vec2 I1N(0,0);/// Momentum of circle one in the normal direction
   Vec2 I2N(0,0);/// Momentum of circle two in the normal direction

   if (I1M > 0.0) {
      I1N = ScalarProjection(I1 , N1);/// Magnitude of N is always 1, they're normalized
   }
   if (I2M > 0.0) {/// Object has momentum
      I2N = ScalarProjection(I2 , N2);/// Magnitude of N is always 1, they're normalized
   }

   if (DotProduct(I1,N1) > 0.0) {
      /// Circle one is moving towards circle two
      /// Give circle ones normal momentum to circle two
      I1 = I1 - I1N;/// This momentum is lost, transferred to the other circle
      I1N *= ELASTICITY;/// Energy is lost due to inelasticity
      if (c2->fixed) {/// We hit an immovable object
         I1N *= -1.0;/// Reflection of normal energy
         I1 += I1N;/// rebound effect
      }
      else {
         I2 = I2 + I1N;/// The remaining momentum is gained by the other circle
      }
   }

   if (DotProduct(I2,N2) > 0.0) {
      /// Circle two is moving towards circle one
      /// Give circle twos normal momentum to circle one
      I2 = I2 - I2N;/// This momentum is lost
      I2N *= ELASTICITY;/// Energy lost due to inelasticity
      if (c1->fixed) {/// We hit an immovable object
         I2N *= -1.0;/// Reflection of normal energy
         I2 += I2N;/// rebound effect
      }
      else {
         I1 = I1 + I2N;/// The remaining momentum is gained by the other circle
      }
   }
///   double Itotal2 = (I1 + I2).Magnitude();
   
   bool changed = false;
   if (!c1->fixed && m1 > 0.0) {
      V1 = I1*(1.0/m1);
      c1->SetSpeed(V1.x , V1.y);
      changed = true;
   }
   if (!c2->fixed && m2 > 0.0) {
      V2 = I2*(1.0/m2);
      c2->SetSpeed(V2.x , V2.y);
      changed = true;
   }
   return changed;
}



CRESOLVER cresolver = MakeCirclesBounce2;



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
   return rsq > dx*dx + dy*dy;
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
///   else if (T2 >= 0.0) {
///      return T2;
///   }
   return -1.0;/// No collision possible in the future
}




