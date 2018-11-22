


#include "LineSeg2.hpp"
#include "Triangles.hpp"
#include "allegro5/allegro_primitives.h"



void Triangle::Draw(ALLEGRO_COLOR c1 , ALLEGRO_COLOR c2 , double thickness) {
   al_draw_filled_triangle(p1.x , p1.y , p2.x , p2.y , p3.x , p3.y , c1);
   al_draw_triangle(p1.x , p1.y , p2.x , p2.y , p3.x , p3.y , c2 , thickness);
}



TRISIDE GetTriSide(Vec2 p , const Triangle& tri) {
   /// Does triangle wind clockwise or counter clockwise?
   LineSeg2 A(tri.p1 , tri.p2);
   LineSeg2 B(tri.p2 , tri.p3);
   LineSeg2 C(tri.p3 , tri.p1);
   RELDIR rd0 = GetSide(tri.p3 , A);
   RELDIR rd1 = GetSide(p , A);
   RELDIR rd2 = GetSide(p , B);
   RELDIR rd3 = GetSide(p , C);
   int score = (int)rd1 + (int)rd2 + (int)rd3;
   if (score == 3 || score == -3) {
      /// All on left or all on right, only possible if inside triangle, can't be outside of all three at the same time
      return TRI_INSIDE;
   }
   if (rd0 == RDRIGHT) {
      if (rd1 == RDLEFT || rd2 == RDLEFT || rd3 == RDLEFT) {
         return TRI_OUTSIDE;
      }
   }
   else if (rd0 == RDLEFT) {
      if (rd1 == RDRIGHT || rd2 == RDRIGHT || rd3 == RDRIGHT) {
         return TRI_OUTSIDE;
      }
   }
   return TRI_ONEDGE;
}
