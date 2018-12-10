



#ifndef LineSeg_HPP
#define LineSeg_HPP


#include "Vector2.hpp"


class LineSeg2 {
public :
   Vec2 p1;
   Vec2 p2;
   
   LineSeg2(Vec2 pt1 , Vec2 pt2) : p1(pt1) , p2(pt2) {}
///   LineSeq2(Vec2 pt1 , Vec2 pt2)
   
   inline void MoveBy(const Vec2& v) {p1 += v;p2 += v;}

};

/**
   If we have a vector defined by p1->p2, or respectively, p2-p1, then we can define anything 'left' of this, 'right', or otherwise 'center'
   also, if we negate or reverse the two points, it should return the opposite value
   We do this by taking the normal and the vector from p1 to the point in question, or p3-p1 and doing their dot product. If it is less than zero, then
   it is left of the vector in question, given its heading.
   
   Now, how do we do this for lines? Ax + By + C = 0 is true, but so is -Ax - By - C = 0
   A is -dy and B is dx, and C is determined by dyx - dxy
*/
enum RELDIR {
   RDLEFT =  -1,
   RDCENTER = 0,
   RDRIGHT =  1
};

RELDIR GetSide(Vec2 p , const LineSeg2& ls);


#endif // LineSeg_HPP
