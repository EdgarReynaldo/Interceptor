



#include "CollTable.hpp"
#include "Intercept.hpp"
#include "Overlap.hpp"
#include "Resolver.hpp"



bool CompareCInfo(const CollInfo* info1 , const CollInfo* info2) {
   return info1->dt < info2->dt;
}



CollInfo::CollInfo() :
   dt(-1.0),
   objects(-1,-1),
   dirty(true)
{}



void CollTable::MarkDirty(unsigned int ND) {
   const unsigned int N = objects.size();
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
   const unsigned int N = objects.size();
   if (ND >= N) {return;}
   
   for (unsigned int c = 0 ; c < ND ; ++c) {
      unsigned int I = InfoIndex(ND , c);
      ctable[I].objects.first = ND;
      ctable[I].objects.second = c;
   }
   for (unsigned int r = ND+1 ; r < N ; ++r) {
      unsigned int I = InfoIndex(r , ND);
      ctable[I].objects.first = r;
      ctable[I].objects.second = ND;
   }
}



int CollTable::InfoIndex(int R , int C) {
   if (R == C) {return -1;}
   if (R < C) {
      int temp = C;
      C = R;
      R = temp;
   }
   const int N = (int)objects.size();
   if (N == 0) {return -1;}
   if (R >= N) {return -1;}
   return (R*R - R)/2 + C;
}



int CollTable::GetIndex(CObject* c) {
   for (unsigned int i = 0 ; i < objects.size() ; ++i) {
      if (objects[i] == c) {
         return i;
      }
   }
   return -1;
}



void CollTable::BuildCollTable() {
   std::sort(objects.begin() , objects.end());///wCircleAddressComparator);
   
   const unsigned int N = objects.size();
   assert(ctable.size() == (N*(N-1))/2);
   unsigned int I = 0;
   for (unsigned int R = 1 ; R < N ; ++R) {
      for (unsigned int C = 0 ; C < N && C < R ; ++C) {/// (N^2 - N)/ 2
         I = (R*(R-1))/2 + C;
         ctable[I].dirty = true;
         ctable[I].objects.first = R;
         ctable[I].objects.second = C;
      }
   }
}



std::vector<CollInfo*> CollTable::GetFirstCollisionsEarlierThanDT(double dt) {
   std::vector<CollInfo*> clist;
   const unsigned int N = ctable.size();
   double min = dt;
   for (unsigned int n = 0 ; n < N ; ++n) {
      CollInfo& info = ctable[n];
      if (info.dt < 0.0) {continue;}
      if (info.dt > min) {continue;}
      if (info.dt < min) {
         clist.clear();
         min = info.dt;
      }
      clist.push_back(&ctable[n]);
   }
   std::sort(clist.begin() , clist.end() , CompareCInfo);
   return clist;
}



void CollTable::ReserveN(unsigned int N) {
   cset.clear();
   objects = std::vector<CObject*>(N , (CObject*)0);
   ctable = std::vector<CollInfo>((N*(N-1))/2 , CollInfo());
   BuildCollTable();
}



void CollTable::SetCircleI(CObject* c , unsigned int I) {
   if (I >= objects.size()) {return;}

   std::unordered_set<CObject*>::iterator it = cset.find(objects[I]);
   if (it != cset.end()) {cset.erase(it);}

   objects[I] = c;

   it = cset.find(c);
   if (it == cset.end()) {cset.insert(c);}

   MarkDirty(I);
}



void CollTable::AddObject(CObject* cnew) {
   std::unordered_set<CObject*>::iterator it = cset.find(cnew);
   if (it != cset.end()) {return;}/// prevent duplicates
   /// Our objects are sorted by address, as that is the order they will be checked in, better for the cache
   /// This means insertion and deletion are obnoxious
   /// Our collision info table is sorted by row and then by column
   /// If we only add new objects at the end of our circle vector, then indices never change
   /// We can mark dead objects as 'inactive' and prevent them from being checked
   /// But to improve cache performance, we want to sort our circle vector by ascending address, to match the traversal order
   /// This means we need to find the insertion point
   
   /// TODO : SORT circle vector by address, see if it improves performance
   
   cset.insert(cnew);
   
   cnew->ctable = this;
   
   /// First, see if we have any dead circles, if so, we can replace them directly, and everything will still Just Work
   const unsigned int N = objects.size();
   for (unsigned int n = 0 ; n < N ; ++n) {
      CObject* c = objects[n];
      if (!c || (c && !c->active)) {
         objects[n] = cnew;
         MarkDirty(n);
         return;
      }
   }
   
   /// We need to add it to the end
   const int NewN2 = ((N+1)*N)/2;

   objects.push_back(cnew);
   
   ctable.resize(NewN2 , CollInfo());
   MarkDirty(N);
   ReNumber(N);
}



void CollTable::RemoveObject(CObject* cold) {
   if (cset.find(cold) == cset.end()) {return;}/// not on list
   if (!cold) {return;}
   cold->ctable = 0;
   objects[GetIndex(cold)] = 0;
}



void CollTable::RecalculateCollTable() {
   if (!ctable_dirty) {return;}
   const unsigned int N = ctable.size();
///   const unsigned int N2 = (N*(N-1))/2;
   for (unsigned int n = 0 ; n < N ; ++n) {
      CollInfo& info = ctable[n];
      CObject* c1 = objects[info.objects.first];
      CObject* c2 = objects[info.objects.second];
      if (info.dirty && c1 && c2) {
         info.dt = GetInterceptTime(*c1 , *c2);
      }
      info.dirty = false;
   }
   ctable_dirty = false;
}
   


int CollTable::UpdateCollisionTableAndResolve(double dt) {
   int overlapcount = 0;
   if (dt <= 0.0) {return 0;}/// Can't currently rewind time, sorry
   
   std::vector<CollInfo*> collisions;
   double dtrem = dt;
   do {
      overlapcount = 0;
      RecalculateCollTable();
      collisions = GetFirstCollisionsEarlierThanDT(dtrem);
      double tfirst = collisions.size()?collisions[0]->dt:dtrem;/// The first collision, or dt if none
      dtrem -= tfirst;
      
      if (collisions.size()) {
         printf("%d collisions at time %1.8lf\n" , (int)collisions.size() , tfirst);
      }
///      assert(tfirst != 0.0);
      
      /// Advance time to first collision
      for (unsigned int ci = 0 ; ci < objects.size() ; ++ci) {
         CObject* c = objects[ci];
         if (c && c->active) {
            c->Update(tfirst);
         }
      }
      for (unsigned int i = 0 ; i < ctable.size() ; ++i) {
         CollInfo* cinfo = &ctable[i];
         cinfo->dt -= tfirst;
      }
/** Output collision table
      const unsigned int N2 = (unsigned int)(int)cset.size()*((int)cset.size()-1)/2;
      for (unsigned int i = 0 ; i < N2  ; ++i) {
         CollInfo* cinfo = &ctable[i];
         printf("#%u %2.4lf " , i , cinfo->dt);
      }
      printf("\n");
*/
      /// Resolve collisions
      for (unsigned int cl = 0 ; cl < collisions.size() ; ++cl) {
         CollInfo* cinfo = collisions[cl];
         if (cinfo->dt != 0.0) {
            printf("Houston we have a problem.\n");
         }
         if (cinfo->dt == 0.0) {
//            printf("collision\n");
            CObject* c1 = objects[cinfo->objects.first];
            CObject* c2 = objects[cinfo->objects.second];
            if (cresolver) {
               if (cresolver(c1 , c2)) {
///                  MarkDirty(GetIndex(c1));
///                  MarkDirty(GetIndex(c2));
               }
            }
         }
      }
      dt = dtrem;
   } while (dt > 0.0);/// && collisions.size());
   return overlapcount;
}



void CollTable::MarkDirty() {
   ctable_dirty = true;
   for (unsigned int i = 0 ; i < ctable.size() ; ++i) {
      ctable[i].dirty = true;
   }
}



void CollTable::MarkDirty(CObject* c) {
   if (c && cset.find(c) != cset.end()) {
      MarkDirty(GetIndex(c));
   }
}
