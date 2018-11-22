



#ifndef Triangles_HPP
#define Triangles_HPP


#include "Vector2.hpp"
#include "allegro5/allegro.h"


class Triangle {
public :
   Vec2 p1,p2,p3;
   
   Triangle(Vec2 pt1 , Vec2 pt2 , Vec2 pt3) : p1(pt1) , p2(pt2) , p3(pt3) {}
   
   void Draw(ALLEGRO_COLOR c1 , ALLEGRO_COLOR c2 , double thickness);
};


enum TRISIDE {
   TRI_INSIDE = -1,
   TRI_ONEDGE =  0,
   TRI_OUTSIDE = 1
};

TRISIDE GetTriSide(Vec2 p , const Triangle& tri);

#endif // Triangles_HPP
