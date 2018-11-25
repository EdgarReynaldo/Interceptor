//---------------------------------------------------------------------------

#ifndef QuadRootsRevJH
#define QuadRootsRevJH
//---------------------------------------------------------------------------

/// #include "CplxD.h"

 #define LDBL_EPSILON 1.084202172485504434E-19L
// #define M_SQRT3 1.7320508075688772935274463L   // sqrt(3)
 #define M_SQRT3_2 0.8660254037844386467637231L   // sqrt(3)/2
// #define DBL_EPSILON  2.2204460492503131E-16    // 2^-52  typically defined in the compiler's float.h
 #define ZERO_PLUS   8.88178419700125232E-16      // 2^-50 = 4*DBL_EPSILON
 #define ZERO_MINUS -8.88178419700125232E-16
 #define TINY_VALUE  1.0E-30                      // This is what we use to test for zero. Usually to avoid divide by zero.
 int QuadCubicRoots(long double *Coeff, int N, long double *RealRoot, long double *ImagRoot);
 void QuadRoots(long double *P, long double *RealPart, long double *ImagPart);
 void CubicRoots(long double *P, long double *RealPart, long double *ImagPart);
 void BiQuadRoots(long double *P, long double *RealPart, long double *ImagPart);
 void ReversePoly(long double *P, int N);
 void InvertRoots(int N, long double *RealRoot, long double *ImagRoot);

#endif


/*
This is a scaled version of QuadRoots. We use b and c because modifying P isn't allowed for BiQuadRoots
void QuadRoots(long double *P, long double *RealRoot, long double *ImagRoot)
{
 long double D, Scalar, b, c;

 if(P[2] == 0.0)
  {
   RealRoot[0] = -P[1];
   RealRoot[1] = ImagRoot[0] = ImagRoot[1] = 0.0;
   return;
  }

 b = P[1];
 if(P[2] > 0.0)c = 1.0;
 else c = -1.0;

 Scalar = sqrtl(fabsl(P[2]));
 b /= Scalar;
 Scalar *= 0.5;

 D = b*b - 4.0*c;
 if(D >= 0.0)  // 2 real roots
  {
   RealRoot[0] = (-b - sqrtl(D)) * Scalar;
   RealRoot[1] = (-b + sqrtl(D)) * Scalar;
   ImagRoot[0] = ImagRoot[1] = 0.0;
  }
 else // 2 complex roots
  {
   RealRoot[0] = RealRoot[1] = -b * Scalar;
   ImagRoot[0] = sqrtl(-D) * Scalar;
   ImagRoot[1] = -ImagRoot[0];
  }
}

*/
