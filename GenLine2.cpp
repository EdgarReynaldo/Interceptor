


#include <cstdio>



#include "GenLine2.hpp"



GenLine2 LineFromPointAngle(const Vec2& p1 , double theta) {
   const double dx = cos(theta);
   const double dy = sin(theta);
   return GenLine2(dy,-dx,dx*p1.y - dy*p1.x);
}



GenLine2 LineFromTwoPoints(const Vec2& p1 , const Vec2& p2) {
   const double dx = p2.x - p1.x;
   const double dy = p2.y - p1.y;
   if (dx == 0.0 && dy == 0.0) {
      printf("Illegal line. Points are equal.\n");
      return GenLine2();
   }
   return GenLine2(dy,-dx,dx*p1.y - dy*p1.x);
}



bool GetIntersection(const GenLine2& l1 , const GenLine2& l2 , Vec2* ppos) {
   const double A = l1.A;
   const double B = l1.B;
   const double C = l1.C;
   const double D = l2.A;
   const double E = l2.B;
   const double F = l2.C;
   const double BDMAE = B*D-A*E;
   if (BDMAE == 0.0) {return false;}
   if (ppos) {
      *ppos = Vec2((C*E-B*F)/BDMAE , (C*D-A*F)/BDMAE);
   }
   return true;
}

