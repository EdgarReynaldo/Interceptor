




#ifndef Object_HPP
#define Object_HPP

#include "Vector2.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include "Arrow.hpp"



class MoveInfo {
public :
   Vec2 pos;
   Vec2 vel;
   Vec2 acc;
   
   MoveInfo() : pos() , vel() , acc() {}
   MoveInfo(Vec2 p , Vec2 v , Vec2 a) : pos(p) , vel(v) , acc(a) {}
   
   inline void Stop() {vel = Vec2(0,0);}
   inline void Reverse() {vel = -vel;}
   inline void ReflectX() {vel = Vec2(-vel.x , vel.y);}
   inline void ReflectY() {vel = Vec2(vel.x , -vel.y);}
   inline void Update(double dt) {
      pos += vel*dt + acc*dt*dt*(0.5);
      vel += acc*dt;
   }
   MoveInfo FutureInfo(double dt);
};

MoveInfo operator-(const MoveInfo& info2 , const MoveInfo& info1);



class RotateInfo {
public :
   double alpha;/// Rotational angle, from pointing straight right, +CCW
   double omega;/// rotational velocity in radians per second
};



class PhysicsInfo {
public :
   double mass;
   double moment;
};
   


class CObject {
public :
   MoveInfo mov;
//   RotateInfo rot;
   PhysicsInfo phys;
   
   
   double rad;/// Bounding radius
   bool fixed;/// Permanent, non-moving, super heavy object
   bool active;/// Live or not

   
///   CObject();
   CObject() :
         mov(),
         phys(),
         rad(0.0),
         fixed(false),
         active(true)
   {}
//   CObject(Vec2 pos , double radius);
   CObject(double xpos , double ypos , double radius) :
         mov(Vec2(xpos,ypos) , Vec2() , Vec2()),
         phys(),
         rad(fabs(radius)),
         fixed(false),
         active(true)
   {}
   
   
   
   
   
   
   
   
   void Draw(ALLEGRO_COLOR col);
   void DrawHollow(ALLEGRO_COLOR col);

   inline void Stop() {mov.Stop();}
   inline void Reverse() {mov.Reverse();}

   inline void SetSpeed(double sx , double sy) {
      mov.vel = Vec2(sx,sy);
   }
   inline void SetAccel(double ax , double ay) {
      mov.acc = Vec2(ax,ay);
   }
   inline void Update(double dt) {
      mov.Update(dt);
   }
   inline MoveInfo FutureInfo(double dt) {return mov.FutureInfo(dt);}
};



#endif // Object_HPP
