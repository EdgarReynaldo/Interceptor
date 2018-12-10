



#ifndef Quartic_HPP
#define Quartic_HPP

/**
   Adapter interface for the P51 quartic solver
*/

#include <vector>

struct QuarticSolution {
   long double rpart[4];
   long double ipart[4];
   unsigned int nroots;
   
   QuarticSolution() : rpart() , ipart() , nroots(0) {}
   
   std::vector<double> GetRealIntercepts();
};

QuarticSolution SolveQuartic(long double coeff[5]);

QuarticSolution SolveQuartic(long double a , long double b , long double c , long double d , long double e);





#endif // Quartic_HPP

