



#include "Object.hpp"


MoveInfo operator-(const MoveInfo& info2 , const MoveInfo& info1) {
   return MoveInfo(info2.pos - info1.pos , info2.vel - info1.vel , info2.acc - info1.acc);
}
