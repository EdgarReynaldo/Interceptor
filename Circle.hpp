



#ifndef Circle_HPP
#define Circle_HPP

#include <algorithm>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include "Arrow.hpp"
extern double ELASTICITY;
extern double EPSILON;


class Circle;

bool Overlaps(const Circle& c1 , const Circle& c2);


double CalculateCollision(Circle* , Circle*);

typedef bool (*CRESOLVER)(Circle* , Circle*);

extern CRESOLVER cresolver;



class Circle {
public :
   double cx,cy,rad;
   double vx,vy;
   bool active;
   bool fixed;/// Fixed, immovable, infinite mass
   double density;
   double mass;
   
   Circle();
   Circle(double x , double y , double r);


   inline bool Overlaps(const Circle& c) {return ::Overlaps(*this , c);}
   
   inline void Draw(ALLEGRO_COLOR col) {
      al_draw_circle(cx , cy , rad , col , 3.0);
      Vec2 c(cx,cy);
      Vec2 v(vx,vy);
      DrawArrow(c , c + v , al_map_rgb(0,0,255));
   }
///   inline void SetSpeed(double sx , double sy);
   inline void SetSpeed(double sx , double sy) {
      vx = sx;
      vy = sy;
   }
   inline void Stop() {
      vx = 0.0;
      vy = 0.0;
   }
   inline void Update(double dt) {
      cx += vx*dt;
      cy += vy*dt;
   }
   inline void Reverse() {
      vx = -vx;
      vy = -vy;
   }
};




#endif // Circle_HPP
