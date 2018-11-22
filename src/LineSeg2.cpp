



#include "LineSeg2.hpp"



RELDIR GetSide(Vec2 p , const LineSeg2& ls) {
   Vec2 l = ls.p2 - ls.p1;
   Vec2 v = p - ls.p1;
   Vec2 l90 = l.Right90().Normalize();
   double dp = DotProduct(v , l90);
   if (dp < 0.0) {
      return RDLEFT;
   } else if (dp > 0.0) {\
      return RDRIGHT;
   }
   return RDCENTER;
}
