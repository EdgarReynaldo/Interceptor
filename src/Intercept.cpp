



#include "Intercept.hpp"

#include <cstdio>



double GetInterceptTime(const CObject& c1 , const CObject& c2) {
   MoveInfo rinfo = c2.mov - c1.mov;/// c1 acts as stationary
   const double rsq = (c1.rad+c2.rad)*(c1.rad+c2.rad);
   const double DSTSQ = rinfo.pos.MagnitudeSquared();
   if (DSTSQ <= rsq) {
      /// Already overlapping, no collision
      return -1.0;
   }
   Vec2 towards = -rinfo.pos;
   towards.Normalize();
   
   Vec2 v = rinfo.vel;
   Vec2 a = rinfo.acc;
   
   /// Relative angle between velocity vector and towards vector and acceleration vector and towards vector
   const double cosv = DotProduct(v , towards);
   const double cosa = DotProduct(a , towards);

   const double VEL = v.Magnitude()*cosv;/// Magnitude of velocity in direction of towards
   const double ACC = a.Magnitude()*cosa;/// Magnitude of acceleration in direction of towards
   const double DST = sqrt(DSTSQ - rsq);
   
   if (ACC == 0.0) {
      if (VEL == 0.0) {
         return -1.0;/// No relative movement
      }
      /// No acceleration
      return DST/VEL;
   }
   
   /// Quadratic equation [ACC*t^2)/2 + VEL*t - DST = 0
   const double DISCRIM = VEL*VEL - 2.0*ACC*DST;
   if (DISCRIM < 0.0) {
      return -1.0;
   }
   const double SQRTD = sqrt(DISCRIM);
   const double T1 = (-VEL - SQRTD)/ACC;
///   const double T2 = (-VEL + SQRTD)/ACC;
   
   if (T1 > 0.0) {
      printf("Collision detected in the future at %1.10lf between %p and %p\n" , T1 , &c1 , &c2);
      return T1;
   }
   return -1.0;
}
