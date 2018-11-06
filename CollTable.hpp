



#ifndef CollTable_HPP
#define CollTable_HPP



#include <algorithm>
#include <unordered_set>
#include <vector>

#include "Circle.hpp"



typedef std::pair<int , int> INDEXPAIR;



class CollInfo {
public :
   double dt;/// -1 if not possible in future, otherwise, intercept time in future
   INDEXPAIR circles;
   bool dirty;
   
   CollInfo();
   
};



bool CompareCInfo(const CollInfo* info1 , const CollInfo* info2);



class CollTable {
   std::unordered_set<Circle*> cset;
   std::vector<Circle*> circles;/// N circle pointers
   std::vector<CollInfo> ctable;/// N*(N-1)/2 pairing of Circle* pairs
   bool ctable_dirty;
   
   void MarkDirty(unsigned int ND);
   void ReNumber(unsigned int ND);


   int InfoIndex(int R , int C);/// 0 based r and c row and column

   int GetIndex(Circle* c);
   
   void BuildCollTable();

   std::vector<CollInfo*> GetFutureCollisionsEarlierThanDT(double dt);

public :
   
   void ReserveN(unsigned int N);

   void SetCircleI(Circle* c , unsigned int I);

   void AddCircle(Circle* cnew);
   
   void RemoveCircle(Circle* cold);



   void RecalculateCollTable();

   void UpdateCollisionTableAndResolve(double dt);

   void MarkDirty();
   
};




#endif // CollTable_HPP
