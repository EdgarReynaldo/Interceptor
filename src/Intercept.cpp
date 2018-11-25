



#include "Intercept.hpp"

#include <cstdio>


#include "Quartic.hpp"



double GetInterceptTime(const CObject& c1 , const CObject& c2) {
   MoveInfo rinfo = c2.mov - c1.mov;/// c1 acts as stationary
   const double rsq = (c1.rad+c2.rad)*(c1.rad+c2.rad);
   const double DSTSQ = rinfo.pos.MagnitudeSquared();
   if (DSTSQ <= rsq) {
      /// Already overlapping, no collision
      printf("overlap.");
      return -1.0;
   }
   Vec2 towards = -rinfo.pos;
   towards.Normalize();

//   Vec2 ortho = towards.Right90();
   
///   Vec2 pos = rinfo.pos;
   Vec2 vel = rinfo.vel;
   Vec2 acc = rinfo.acc;
   
   /// Relative angle between velocity vector and towards vector and acceleration vector and towards vector
   const double dpv = DotProduct(vel , towards);
   const double dpa = DotProduct(acc , towards);

   Vec2 veln = towards*dpv;
   Vec2 accn = towards*dpa;
///   const double DST = sqrt(DSTSQ - rsq);
   
   DrawArrow(c2.mov.pos , c2.mov.pos + veln , al_map_rgb(255,0,0));
   DrawArrow(c2.mov.pos , c2.mov.pos + accn , al_map_rgb(255,0,255));
   
   const double VEL = veln.Magnitude();/// Magnitude of velocity in direction of towards
   const double ACC = accn.Magnitude();/// Magnitude of acceleration in direction of towards

   if (ACC == 0.0) {
      /// No relative normal acceleration
      if (VEL <= 0.0) {
         return -1.0;/// Moving away
      }
      
      /// Need to get the quadratic solution here
///      const double a = 1.0;
      const double VSQ = vel.MagnitudeSquared();
      const double b = 2.0*DotProduct(rinfo.pos , rinfo.vel)/VSQ;
      const double c = (DSTSQ - rsq)/VSQ;
      const double discrim = b*b - 4.0*c;/// a is 1
      if (discrim < 0.0) {
         printf("No real roots.");
         return -1.0;
      }
      const double sqrtd = sqrt(discrim);
      const double T1 = (-b - sqrtd)*0.5;/// a is 1
      const double T2 = (-b + sqrtd)*0.5;/// a is 1
      if (T1 >= 0.0) {return T1;}
      else if (T2 >= 0.0) {
         /// if T1 is negative and T2 is positive, then they are already colliding
///         return T2;
      }
      return -1.0;
      
   } else if (ACC < 0.0) {
      /// Accelerating away
      if (VEL <= 0.0) {
         return -1.0;/// Moving away
      }
      else {
         /// Moving towards, accelerating away
         /// TODO : See if it is possible for the object to move as far as the distance between them
         (void)0;
      }
   } else {
      /// Accelerating towards c1
   }
   
   /// Two accelerating balls can be modeled as the intersection of a parabola and a circle
   /// The relative position between the two circles centers follows a parabolic path,
   /// And the distance from the origin is represented by a circle with a radius that
   /// is the sum of the two circles radii. If instead we wanted to find the intercept time of
   /// One ball inside another we set the distance between them equal to the difference of their radii
   
   /// The parametric parabolic equation is given by the position function on t, which is as follows
   /// Px(t) = Px0 + Vx0*t + Ax0*t^2 / 2
   /// Py(t) = Py0 + Vy0*t + Ay0*t^2 / 2
   /// And the distance function D on P is ||P->|| which is D(P(t)) = sqrt(Px^2 + Py^2)
   /// We want to know when the distance between the two circles centers (given by D(P(t)))
   /// Is equal to the sum of their radii. That is to say, when their difference is zero.
   
   /// So we have D(P(t)) - (r1 + r2) = 0 and D(P(t)) = (r1 + r2) = sqrt(Px^2 + Py^2)
   /// If we square both sides we get (r1 + r2)^2 = (Px^2 + Py^2) and (Px^2 + Py^2) - (r1 + r2)^2 = 0
   /// When you multiply out the square of Px(t) and Py(t) It works out to this mess :
   /// 
   /// t^4(Ax^2 + Ay^2) + 
   /// t^3(VxAx + VyAy) + 
   /// t^2(Vx^2 + Vy^2 + PxAx + PyAy) + 
   /// t^1(2(PxVx + PyVy)) + 
   /// t^0(Px^2 + Py^2 - (r1 + r2)^2
   /// = 0
   /// 
   /// Which is actually pretty awesome because all these are are magnitudes squared and dot products
   /// of the combinations of position, velocity, and acceleration. Math really is beautiful.
   
   /// This is a quartic equation and this is where P51 comes in handy with its quartic solver
   /// Find values for a,b,c,d, and e for which ax^4 + bx^3 + cx^2 + dx + e = 0
   
   /// We need to normalize the equation so that a is 1 - to do this divide b,c,d, and e by a
   const long double ACCSQ = rinfo.acc.MagnitudeSquared();
   assert(ACCSQ > 0.0);/// precondition
   const long double a = 1.0L;/// ACCSQ divided by itself is 1
   const long double b = DotProduct(rinfo.vel , rinfo.acc) / ACCSQ;
   const long double c = (rinfo.vel.MagnitudeSquared() + DotProduct(rinfo.pos , rinfo.acc))/ACCSQ;
   const long double d = DotProduct(rinfo.pos , rinfo.vel)/ACCSQ;
   const long double e = (DSTSQ - rsq)/ACCSQ;
   
   QuarticSolution qs = SolveQuartic(a,b,c,d,e);
   
   return qs.GetRealIntercept();
   
/**   
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
*/
   return -1.0;
}



double GetInterceptTimeOld(const CObject& c1 , const CObject& c2) {
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
