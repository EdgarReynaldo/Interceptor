



#ifndef Resolver_HPP
#define Resolver_HPP

#include "Object.hpp"



extern double ELASTICITY;/// [0.0 , 1.0] - how much energy is retained after a collision
extern double EPSILON;/// For approximating 0 - currently unused

typedef bool (*CRESOLVER)(CObject* , CObject*);

extern CRESOLVER cresolver;


#endif // Resolver_HPP
