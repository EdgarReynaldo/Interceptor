


#include "QuadRootsRevJ.h"

#include "Quartic.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>


const long double EPSILON2 = 0.0001L;

std::vector<double> QuarticSolution::GetRealIntercepts() {
   std::vector<double> times;
   for (unsigned int i = 0 ; i < nroots ; ++i) {
      if (ipart[i] == 0.0L) {
         times.push_back((double)rpart[i]);
///         printf("Collision time detected in quartic solution : %2.8lf\n" , (double)rpart[i]);
      }
   }
   std::sort(times.begin() , times.end());
///   return times.size()?times[0]:-1.0;
   return times;///.size()?times[0]:-1.0;
}



QuarticSolution SolveQuartic(long double coeff[5]) {
   QuarticSolution s;
/// int QuadCubicRoots(long double *Coeff, int N, long double *RealRoot, long double *ImagRoot);
   s.nroots = QuadCubicRoots(coeff , 4 , s.rpart , s.ipart);
///   printf("%u roots returned by QuadCubicRoots\n" , s.nroots);
   for (unsigned int i = 0 ; i < s.nroots ; ++i) {
///      printf("[%lf , %lf*i] " , (double)s.rpart[i] , (double)s.ipart[i]);
   }
///   printf("\n");
   return s;
}



QuarticSolution SolveQuartic(long double a , long double b , long double c , long double d , long double e) {
   long double coeff[5] = {a,b,c,d,e};
   return SolveQuartic(coeff);
}
