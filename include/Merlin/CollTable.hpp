



#ifndef CollTable_HPP
#define CollTable_HPP



#include <algorithm>
#include <unordered_set>
#include <vector>

///#include "Object.hpp"


class CObject;



typedef std::pair<int , int> INDEXPAIR;



class CollInfo {
public :
   double dt;/// -1 if not possible in future, otherwise, intercept time in future
   INDEXPAIR objects;
   bool dirty;
   
   CollInfo();
   
};



bool CompareCInfo(const CollInfo* info1 , const CollInfo* info2);



class CollTable {
   std::unordered_set<CObject*> cset;
   std::vector<CObject*> objects;/// N circle pointers
   std::vector<CollInfo> ctable;/// N*(N-1)/2 pairing of Circle* pairs
   bool ctable_dirty;
   
   void MarkDirty(unsigned int ND);
   void ReNumber(unsigned int ND);


   int InfoIndex(int R , int C);/// 0 based r and c row and column

   int GetIndex(CObject* c);
   
   void BuildCollTable();

   std::vector<CollInfo*> GetFirstCollisionsEarlierThanDT(double dt);

public :
   
   void ReserveN(unsigned int N);

   void SetCircleI(CObject* c , unsigned int I);

   void AddObject(CObject* cnew);
   
   void RemoveObject(CObject* cold);



   void RecalculateCollTable();

   int UpdateCollisionTableAndResolve(double dt);

   void MarkDirty();
   void MarkDirty(CObject* c);
   
};




#endif // CollTable_HPP
