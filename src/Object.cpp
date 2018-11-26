



#include "Object.hpp"


MoveInfo MoveInfo::FutureInfo(double dt) const {
   MoveInfo f = *this;
   f.Update(dt);
   return f;
}



MoveInfo operator-(const MoveInfo& info2 , const MoveInfo& info1) {
   return MoveInfo(info2.pos - info1.pos , info2.vel - info1.vel , info2.acc - info1.acc);
}



void CObject::Draw(ALLEGRO_COLOR col) {
   al_draw_filled_circle(mov.pos.x , mov.pos.y , rad , al_map_rgb(0,0,0));
   al_draw_circle(mov.pos.x , mov.pos.y , rad , col , 3.0);
   DrawArrow(mov.pos , mov.pos + mov.vel , al_map_rgb(0,255,0));
   DrawArrow(mov.pos , mov.pos + mov.acc , al_map_rgb(255,0,255));
}



void CObject::DrawHollow(ALLEGRO_COLOR col) {
///      al_draw_filled_circle(cx , cy , rad , al_map_rgb(0,0,0));
   al_draw_circle(mov.pos.x , mov.pos.y , rad , col , 3.0);
   DrawArrow(mov.pos , mov.pos + mov.vel , al_map_rgb(0,0,255));
}
