



#ifndef LineSeg_HPP
#define LineSeg_HPP


#include "Vector2.hpp"

class LineSeg2 {
public :
   Vec2 p1;
   Vec2 p2;
   
   inline void MoveBy(const Vec2& v) {p1 += v;p2 += v;}
};






#endif // LineSeg_HPP
