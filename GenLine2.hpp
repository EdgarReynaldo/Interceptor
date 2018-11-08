



#ifndef GenLine2_HPP
#define GenLine2_HPP


#include "LineSeg2.hpp"


class GenLine2 {
public :
   double A,B,C;
   
   GenLine2() : A(0.0) , B(0.0) , C(0.0) {}
   GenLine2(double lA , double lB , double lC) : A(lA) , B(lB) , C(lC) {}
   
   inline bool Valid() {return A != 0.0 && B != 0.0;}
};


GenLine2 LineFromPointAngle(const Vec2& p1 , double theta);
GenLine2 LineFromTwoPoints(const Vec2& p1 , const Vec2& p2);

bool GetIntersection(const GenLine2& l1 , const GenLine2& l2 , Vec2* ppos);



#endif // GenLine2_HPP
