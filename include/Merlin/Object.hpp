




#ifndef Object_HPP
#define Object_HPP

#include "Vector2.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include "Arrow.hpp"

#include "CollTable.hpp"


///class CollTable;

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
   MoveInfo FutureInfo(double dt) const;
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
private :
   unsigned int id;
   static unsigned int NextId();
protected :
   MoveInfo mov;
//   RotateInfo rot;
   PhysicsInfo phys;
public :
   
   double rad;/// Bounding radius
   bool fixed;/// Permanent, non-moving, super heavy object
   bool active;/// Live or not

   
   CollTable* ctable;
   
///   CObject();
   CObject() :
         id(NextId()),
         mov(),
         phys(),
         rad(0.0),
         fixed(false),
         active(true),
         ctable(0)
   {}
//   CObject(Vec2 pos , double radius);
   CObject(double xpos , double ypos , double radius) :
         id(NextId()),
         mov(Vec2(xpos,ypos) , Vec2() , Vec2()),
         phys(),
         rad(fabs(radius)),
         fixed(false),
         active(true),
         ctable(0)
   {
      phys.mass = M_PI*rad*rad;
   }
   
   
   
   
   
   
   
   
   void Draw(ALLEGRO_COLOR col);
   void DrawHollow(ALLEGRO_COLOR col);

   inline void Stop() {mov.Stop();}
   inline void Reverse() {mov.Reverse();}

   inline void SetPos(const Vec2& p) {
      mov.pos = p;
      if (ctable) {
         ctable->MarkDirty(this);
      }
   }
   inline void SetPos(double px , double py) {
      mov.pos = Vec2(px,py);
      if (ctable) {
         ctable->MarkDirty(this);
      }
   }
   inline void SetSpeed(double sx , double sy) {
      mov.vel = Vec2(sx,sy);
      if (ctable) {
         ctable->MarkDirty(this);
      }
   }
   inline void SetAccel(double ax , double ay) {
      mov.acc = Vec2(ax,ay);
      if (ctable) {
         ctable->MarkDirty(this);
      }
   }
   inline void SetMove(MoveInfo info) {mov = info;}
   inline void Update(double dt) {
      mov.Update(dt);
   }
   inline MoveInfo FutureInfo(double dt) const {return mov.FutureInfo(dt);}
   
   inline Vec2 Pos() const {return mov.pos;}
   inline Vec2 Speed() const {return mov.vel;}
   inline Vec2 Accel() const {return mov.acc;}
   
   inline MoveInfo Mov() const {return mov;}
   inline PhysicsInfo Phys() const {return phys;}
   
   inline unsigned int Id() {return id;}
   
   friend bool Overlaps(const CObject& c1 , const CObject& c2);
   friend bool MakeObjectsBounce(CObject* c1 , CObject* c2);
};



#endif // Object_HPP
