



#include "Overlap.hpp"


bool Overlaps(const CObject& c1 , const CObject& c2) {
   const double rsq = c1.rad*c1.rad + c2.rad*c2.rad;
   const double dx = c2.mov.pos.x - c1.mov.pos.x;
   const double dy = c2.mov.pos.y - c1.mov.pos.y;
   const bool coverlap = rsq > dx*dx + dy*dy;
   return coverlap;
}
