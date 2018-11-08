



#include "Arrow.hpp"
#include "allegro5/allegro_primitives.h"


void DrawArrow(Vec2 tail , Vec2 head , ALLEGRO_COLOR col) {
   Vec2 path = head - tail;
   const double a = atan2(path.y , path.x);
   const double dtheta = 2.0*M_PI/3.0;
   const double a1 = a + dtheta;
   const double a2 = a - dtheta;
   const double l = path.Magnitude()/10.0;
   Vec2 v1 = VectorA(a1 , l);
   Vec2 v2 = VectorA(a2 , l);
   v1 += head;
   v2 += head;
   al_draw_line(tail.x , tail.y , head.x , head.y , col , 3.0);
   al_draw_line(v1.x , v1.y , head.x , head.y , col , 3.0);
   al_draw_line(v2.x , v2.y , head.x , head.y , col , 3.0);
}
