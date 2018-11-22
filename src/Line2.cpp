


#include <cstdio>



#include "Line2.hpp"

#include <cassert>



Line2 LineFromPointAngle(const Vec2& p1 , double theta) {
   const double dx = cos(theta);
   const double dy = sin(theta);
   return Line2(dy,-dx,dx*p1.y - dy*p1.x);
}



Line2 LineFromTwoPoints(const Vec2& p1 , const Vec2& p2) {
   const double dx = p2.x - p1.x;
   const double dy = p2.y - p1.y;
   if (dx == 0.0 && dy == 0.0) {
      printf("Illegal line. Points are equal.\n");
      return Line2();
   }
   return Line2(dy,-dx,dx*p1.y - dy*p1.x);
}



bool GetIntersection(const Line2& l1 , const Line2& l2 , Vec2* ppos) {
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



Vec2 GetClosestPointTo(const Line2& l , Vec2 pt) {
   const double DX = -l.DY();
   const double DY = l.DX();
   
   Line2 perp(DY , -DX , DX*pt.y - DY*pt.x);
   Vec2 ret;
   assert(GetIntersection(l , perp , &ret));
   return ret;
}





