



#ifndef Line2_HPP
#define Line2_HPP


#include "LineSeg2.hpp"


class Line2 {
public :
   double A,B,C;
   
   Line2() : A(0.0) , B(0.0) , C(0.0) {}
   Line2(double lA , double lB , double lC) : A(lA) , B(lB) , C(lC) {}
   
   double DY() const {return A;}
   double DX() const {return -B;}
   
   inline bool Valid() {return A != 0.0 && B != 0.0;}
};

/*
bool PointOnLine(double px , double py);
bool LinesEqual(const Line2& l1 , const Line2& l2);
bool PointOnLine(double px , double py) {
   
}
bool LinesEqual(const Line2& l1 , const Line2& l2) {
   
}
*/
Line2 LineFromPointAngle(const Vec2& p1 , double theta);
Line2 LineFromTwoPoints(const Vec2& p1 , const Vec2& p2);

///Line2 LineFromSegment(const LineSeg2& ls);

inline Line2 LineFromSegment(const LineSeg2& ls) {return LineFromTwoPoints(ls.p1 , ls.p2);}

bool GetIntersection(const Line2& l1 , const Line2& l2 , Vec2* ppos);

Vec2 GetClosestPointTo(const Line2& l , Vec2 pt);






#endif // Line2_HPP
