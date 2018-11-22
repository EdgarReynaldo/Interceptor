



#include "Resolver.hpp"



#include "Vector2.hpp"


double ELASTICITY = 1.0;/// No energy lost
//double ELASTICITY = 0.5;/// Half energy lost

double EPSILON = 0.0001;



bool MakeObjectsStick(CObject* c1 , CObject* c2) {
   if (!c1 || !c2) {return false;}
   c1->Stop();
   c2->Stop();
   return true;
}



bool MakeObjectsReverse(CObject* c1 , CObject* c2) {
   if (!c1 || !c2) {return false;}
   c1->Reverse();
   c2->Reverse();
   return true;
}



bool MakeObjectsBounce(CObject* c1 , CObject* c2) {

   /// Normalized normal vectors, these point from one circle towards the other
   Vec2 N1 = (c2->mov.pos - c1->mov.pos).Normalize();
   Vec2 N2 = -N1;
   
   const double m1 = c1->phys.mass;
   const double m2 = c2->phys.mass;

   Vec2 I1 = c1->mov.vel*m1;
   Vec2 I2 = c2->mov.vel*m2;
///   double Itotal = (I1 + I2).Magnitude();
   
   /// The angle between V and N determines how much energy is transferred
   
   const double I1MSQ = I1.MagnitudeSquared();
   const double I2MSQ = I2.MagnitudeSquared();

   Vec2 I1N(0,0);/// Momentum of circle one in the normal direction
   Vec2 I2N(0,0);/// Momentum of circle two in the normal direction

   if (I1MSQ > 0.0) {
      I1N = ScalarProjection(I1 , N1);/// Magnitude of N is always 1, they're normalized
   }
   if (I2MSQ > 0.0) {/// Object has momentum
      I2N = ScalarProjection(I2 , N2);/// Magnitude of N is always 1, they're normalized
   }

   if (DotProduct(I1,N1) > 0.0) {
      /// Circle one is moving towards circle two
      /// Give circle ones normal momentum to circle two
      I1 = I1 - I1N;/// This momentum is lost, transferred to the other circle
      I1N *= ELASTICITY;/// Energy is lost due to inelasticity
      if (c2->fixed) {/// We hit an immovable object
         I1N *= -1.0;/// Reflection of normal energy
         I1 += I1N;/// rebound effect
      }
      else {
         I2 = I2 + I1N;/// The remaining momentum is gained by the other circle
      }
   }

   if (DotProduct(I2,N2) > 0.0) {
      /// Circle two is moving towards circle one
      /// Give circle twos normal momentum to circle one
      I2 = I2 - I2N;/// This momentum is lost
      I2N *= ELASTICITY;/// Energy lost due to inelasticity
      if (c1->fixed) {/// We hit an immovable object
         I2N *= -1.0;/// Reflection of normal energy
         I2 += I2N;/// rebound effect
      }
      else {
         I1 = I1 + I2N;/// The remaining momentum is gained by the other circle
      }
   }
///   double Itotal2 = (I1 + I2).Magnitude();
   
   bool changed = false;
   if (!c1->fixed && m1 > 0.0) {
      Vec2 V1 = I1*(1.0/m1);
      c1->SetSpeed(V1.x , V1.y);
      changed = true;
   }
   if (!c2->fixed && m2 > 0.0) {
      Vec2 V2 = I2*(1.0/m2);
      c2->SetSpeed(V2.x , V2.y);
      changed = true;
   }
   return changed;
}



CRESOLVER cresolver = MakeObjectsBounce;

