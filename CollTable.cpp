



#include "CollTable.hpp"



bool CompareCInfo(const CollInfo* info1 , const CollInfo* info2) {
   return info1->dt < info2->dt;
}



CollInfo::CollInfo() :
   dt(-1.0),
   circles(-1,-1),
   dirty(true)
{}



void CollTable::MarkDirty(unsigned int ND) {
   const unsigned int N = circles.size();
   if (ND >= N) {return;}
   
   for (unsigned int c = 0 ; c < ND ; ++c) {
      unsigned int I = InfoIndex(ND , c);
      ctable[I].dirty = true;
   }
   for (unsigned int r = ND+1 ; r < N ; ++r) {
      unsigned int I = InfoIndex(r , ND);
      ctable[I].dirty = true;
   }
   ctable_dirty = true;
}



void CollTable::ReNumber(unsigned int ND) {
   const unsigned int N = circles.size();
   if (ND >= N) {return;}
   
   for (unsigned int c = 0 ; c < ND ; ++c) {
      unsigned int I = InfoIndex(ND , c);
      ctable[I].circles.first = ND;
      ctable[I].circles.second = c;
   }
   for (unsigned int r = ND+1 ; r < N ; ++r) {
      unsigned int I = InfoIndex(r , ND);
      ctable[I].circles.first = r;
      ctable[I].circles.second = ND;
   }
}



int CollTable::InfoIndex(int R , int C) {
   if (R == C) {return -1;}
   if (R < C) {
      int temp = C;
      C = R;
      R = temp;
   }
   const int N = (int)circles.size();
   if (N == 0) {return -1;}
   if (R >= N) {return -1;}
   return (R*R - R)/2 + C;
}



int CollTable::GetIndex(Circle* c) {
   for (unsigned int i = 0 ; i < circles.size() ; ++i) {
      if (circles[i] == c) {
         return i;
      }
   }
   return -1;
}



void CollTable::BuildCollTable() {
   std::sort(circles.begin() , circles.end());///wCircleAddressComparator);
   
   const unsigned int N = circles.size();
   assert(ctable.size() == (N*(N-1))/2);
   unsigned int I = 0;
   for (unsigned int R = 1 ; R < N ; ++R) {
      for (unsigned int C = 0 ; C < N && C < R ; ++C) {/// (N^2 - N)/ 2
         I = (R*(R-1))/2 + C;
         ctable[I].dirty = true;
         ctable[I].circles.first = R;
         ctable[I].circles.second = C;
      }
   }
}



std::vector<CollInfo*> CollTable::GetFirstCollisionsEarlierThanDT(double dt) {
   std::vector<CollInfo*> clist;
   const unsigned int N = ctable.size();
   double first = -1.0;
   for (unsigned int n = 0 ; n < N ; ++n) {
      CollInfo& info = ctable[n];
      if (info.dt < 0.0) {continue;}
      if (info.dt > dt) {continue;}
      if (first < 0.0) {
         first = info.dt;
      }
      if (info.dt > first) {
         break;
      }
      clist.push_back(&ctable[n]);
   }
   std::sort(clist.begin() , clist.end() , CompareCInfo);
   return clist;
}



void CollTable::ReserveN(unsigned int N) {
   cset.clear();
   circles = std::vector<Circle*>(N , (Circle*)0);
   ctable = std::vector<CollInfo>((N*(N-1))/2 , CollInfo());
   BuildCollTable();
}



void CollTable::SetCircleI(Circle* c , unsigned int I) {
   if (I >= circles.size()) {return;}

   std::unordered_set<Circle*>::iterator it = cset.find(circles[I]);
   if (it != cset.end()) {cset.erase(it);}

   circles[I] = c;

   it = cset.find(c);
   if (it == cset.end()) {cset.insert(c);}

   MarkDirty(I);
}



void CollTable::AddCircle(Circle* cnew) {
   std::unordered_set<Circle*>::iterator it = cset.find(cnew);
   if (it != cset.end()) {return;}/// prevent duplicates
   /// Our circles are sorted by address, as that is the order they will be checked in, better for the cache
   /// This means insertion and deletion are obnoxious
   /// Our collision info table is sorted by row and then by column
   /// If we only add new circles at the end of our circle vector, then indices never change
   /// We can mark dead circles as 'inactive' and prevent them from being checked
   /// But to improve cache performance, we want to sort our circle vector by ascending address, to match the traversal order
   /// This means we need to find the insertion point
   
   /// TODO : SORT circle vector by address, see if it improves performance
   
   cset.insert(cnew);
   
   /// First, see if we have any dead circles, if so, we can replace them directly, and everything will still Just Work
   const unsigned int N = circles.size();
   for (unsigned int n = 0 ; n < N ; ++n) {
      Circle* c = circles[n];
      if (!c || (c && !c->active)) {
         circles[n] = cnew;
         MarkDirty(n);
         return;
      }
   }
   
   /// We need to add it to the end
   const int NewN2 = ((N+1)*N)/2;

   circles.push_back(cnew);
   
   ctable.resize(NewN2 , CollInfo());
   MarkDirty(N);
   ReNumber(N);
}



void CollTable::RemoveCircle(Circle* cold) {
   if (cset.find(cold) == cset.end()) {return;}/// not on list
   if (!cold) {return;}
   circles[GetIndex(cold)] = 0;
}



void CollTable::RecalculateCollTable() {
   if (!ctable_dirty) {return;}
   const unsigned int N = ctable.size();
///   const unsigned int N2 = (N*(N-1))/2;
   for (unsigned int n = 0 ; n < N ; ++n) {
      CollInfo& info = ctable[n];
      if (info.dirty) {
         info.dt = CalculateCollision(circles[info.circles.first] , circles[info.circles.second]);
      }
      info.dirty = false;
   }
   ctable_dirty = false;
}
   


void CollTable::UpdateCollisionTableAndResolve(double dt) {
   if (dt <= 0.0) {return;}
   
   std::vector<CollInfo*> collisions;
   double dtrem = dt;
   do {
      RecalculateCollTable();
      collisions = GetFirstCollisionsEarlierThanDT(dt);
      double tfirst = collisions.size()?collisions[0]->dt:dtrem;/// The first collision, or dt if none
      dtrem -= tfirst;
      
      for (unsigned int ci = 0 ; ci < circles.size() ; ++ci) {
         Circle* c = circles[ci];
         if (c && c->active) {
            c->Update(tfirst);
         }
      }
      for (unsigned int i = 0 ; i < ctable.size() ; ++i) {
         CollInfo* cinfo = &ctable[i];
         cinfo->dt -= tfirst;
      }
      
      for (unsigned int cl = 0 ; cl < collisions.size() ; ++cl) {
         CollInfo* cinfo = collisions[cl];
         if (cinfo->dt == 0.0) {
            printf("collision\n");
            Circle* c1 = circles[cinfo->circles.first];
            Circle* c2 = circles[cinfo->circles.second];
            if (cresolver) {
               if (cresolver(c1 , c2)) {
                  MarkDirty(GetIndex(c1));
                  MarkDirty(GetIndex(c2));
               }
            }
         }
      }
      dt = dtrem;
   } while ((dt > 0.0) && collisions.size());
}



void CollTable::MarkDirty() {
   ctable_dirty = true;
   for (unsigned int i = 0 ; i < ctable.size() ; ++i) {
      ctable[i].dirty = true;
   }
}
