



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


/** MakeObjectsBounce only works correctly for objects with equal mass */
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
         
         /** TODO : IMPORTANT - we're modifying I2 before passing it to the dot product - that's bad*/
         
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



void MakeObjectsConserve(CObject* c1 , CObject* c2) {

   if (c1->fixed && c2->fixed) {return;}
   
   /// Normalized normal vectors, these point from one circle towards the other
   const Vec2 N1 = (c2->Mov().pos - c1->Mov().pos).Normalize();
   const Vec2 N2 = -N1;
   
   const double m1 = c1->Phys().mass;
   const double m2 = c2->Phys().mass;
   const double mtotal = m1 + m2;
   
   assert(mtotal > 0.0);
   
   const Vec2 V1 = c1->Mov().vel;
   const Vec2 V2 = c2->Mov().vel;

   const Vec2 I1 = V1*m1;
   const Vec2 I2 = V2*m2;
  
   /// The angle between V and N determines how much energy is transferred
   const double dp1n = DotProduct(V1 , N1);
   const double dp2n = DotProduct(V2 , N2);
   
   /// |A||B|cos a = A dot B
   /// |B| = 1
   /// ^B*(A dot ^B) = Scalar projection of A unto B
   /// These are our normal velocities
   const Vec2 V1N = N1*dp1n;
   const Vec2 V2N = N2*dp2n;

/**
v1b = (v1*(m1-m2) + 2m2v2)/(m1 + m2)
v2b = (v2(m2-m1) + 2m1v1)/(m1 + m2)
*/
   
   const Vec2 V1NB = (V1N*(m1-m2) + V2N*2.0*m2)*(1.0/(mtotal))*ELASTICITY;
   const Vec2 V2NB = (V2N*(m2-m1) + V1N*2.0*m1)*(1.0/(mtotal))*ELASTICITY;
   
   /// If m1 is zero, we get v1nb = (-m2*v1n + v2n*2*m2)/m2 = 2*v2n - v1n
   ///                   and v2nb = (m2*v2n + 0)/m2 = v2n - unchanged
   
   
   
   const Vec2 NewV1 = V1 - V1N + V1NB;
   const Vec2 NewV2 = V2 - V2N + V2NB;

   
   if (!c1->fixed) {
      c1->SetSpeed(NewV1.x , NewV1.y);
   }
   if (!c2->fixed) {
      c2->SetSpeed(NewV2.x , NewV2.y);
   }
/**
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
*/
}


CRESOLVER cresolver = MakeObjectsConserve;

