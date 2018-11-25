//---------------------------------------------------------------------------

#pragma hdrstop

#include "P51TestCode.h"
#include <cmath>
#include <new>   // Defines new(std::nothrow)
#include <cstdio>   // This defines the ShowMessage() function

#pragma package(smart_init)

//---------------------------------------------------------------------------

// This code is not used by the P51 root finder.
// It is included with P51 for test purposes only.

// If you test P51, or any other root finder, remember that the highest order coefficient
// P[0], must be 1.0 and the last coefficient P[N], must not equal zero.
// If P[0] != 1, P51 will first normalize the coefficients. This may confuse
// your test routine if you are comparing coefficients.

// This function is used for testing a root finder. There are 2 ways to test a root finder.
// First, you can generate a set of roots and build a poly, send the poly to the root finder,
// then compare the original roots to the found roots. This is the best way to test the
// accuracy of the root finder.

// The second way is to build a polynomial from a set of random coefficients, find the roots,
// then build a poly from the found roots, and compare the rebuilt poly coefficients to the
// originals. This works OK, but the results are not as meaningful.

// Use this function to build a polynomial from a set of complex roots. It returns an
// error value, which is the sum of imaginary part of coefficients.
// The return value should be be tiny. i.e. The poly coeff should be real.
long double BuildPolyFromRoots(int N, long double *RealRoot, long double *ImagRoot, long double *PolyCoeff)
{
 int j, k, n;
 long double ImagErrSum, *RealSum, *ImagSum, *RealCoeff, *ImagCoeff;

 RealSum = new(std::nothrow) long double[N+2];
 ImagSum = new(std::nothrow) long double[N+2];
 RealCoeff  = new(std::nothrow) long double[N+2];
 ImagCoeff  = new(std::nothrow) long double[N+2];

 if(RealSum == NULL || ImagSum == NULL || RealCoeff == NULL || ImagCoeff == NULL )
  {
   printf("Memory not Allocated in BuildPolyFromRoots.");
   return(0);
  }

 // Initialize
 for(j=0; j<=N; j++)
  {
   RealCoeff[j] = ImagCoeff[j] = 0.0;
   RealSum[j] = ImagSum[j] = 0.0;
  }

 RealCoeff[0] = -RealRoot[0];
 ImagCoeff[0] = -ImagRoot[0];
 RealCoeff[1] = 1.0;
 ImagCoeff[1] = 0.0;

 // Rebuild the poly from the roots.
 n = 1;
 for(j=1; j<N; j++)
  {
   for(k=0; k<=n; k++)
    {
     RealSum[k] = RealCoeff[k] * -RealRoot[j] - ImagCoeff[k] * -ImagRoot[j];
     ImagSum[k] = RealCoeff[k] * -ImagRoot[j] + ImagCoeff[k] * -RealRoot[j];
    }
   for(k=0; k<=n; k++)
    {
     RealSum[k+1] += RealCoeff[k];
     ImagSum[k+1] += ImagCoeff[k];
    }
   n++;
   for(k=0; k<=n; k++)
    {
     RealCoeff[k] = RealSum[k];
     ImagCoeff[k] = ImagSum[k];
    }
   for(k=0; k<=N; k++)
    {
     RealSum[k] = 0.0;
     ImagSum[k] = 0.0;
    }
  }

 // The poly coeff are the real part. The imag part should be tiny.
 for(k=0; k<=n; k++)PolyCoeff[k] = RealCoeff[n-k];

 // Use ImagErrSum to find mistakes. For example, if the roots only
 // contain one half of a complex pair, ImagErrSum will not be tiny.
 ImagErrSum = 0.0;
 for(j=0; j<=n; j++)
  {
   // This sum should be on the order of 1.E-18, but tiny coeff can exaggerate this sum.
   if(fabsl(RealCoeff[j]) > 1.0E-5)ImagErrSum += fabsl(ImagCoeff[j]/RealCoeff[j]);
  }
 ImagErrSum /= (double)(n+1);

 delete[] RealSum;
 delete[] ImagSum;
 delete[] RealCoeff;
 delete[] ImagCoeff;

 return( ImagErrSum );
}



//---------------------------------------------------------------------------

// The best way to test a root finder is to build a poly from a set of known roots and let the
// root finder find them. Then you can compare the found roots to the originals and test the
// accuracy of the results. The problem is that the original and found roots will not be in
// the same order. Use this function align them for comparison testing.

void SortRoots(int N, long double *OrigRealRoot, long double *OrigImagRoot, long double *RealRoot, long double *ImagRoot)
{
 int j, k, MinK;
 long double Err, MinErr;
 long double *TempReal, *TempImag;

 TempReal  = new(std::nothrow) long double[N+2];
 TempImag  = new(std::nothrow) long double[N+2];
 if(TempReal == NULL || TempImag == NULL  )
  {
   printf("Memory not Allocated in SortRoots.");
   return;
  }

 for(j=0; j<N; j++)TempReal[j] = RealRoot[j];
 for(j=0; j<N; j++)TempImag[j] = ImagRoot[j];

 for(j=0; j<N; j++)
  {
   MinErr = 1.0E100;
   for(k=0; k<N; k++)
    {
     Err =  fabsl(OrigRealRoot[j] - TempReal[k]);
     Err += fabsl(OrigImagRoot[j] - TempImag[k]);
     if(Err < MinErr)
      {
       MinErr = Err;
       MinK = k;
      }
    }
   RealRoot[j] = TempReal[MinK];
   ImagRoot[j] = TempImag[MinK];
   TempReal[MinK] = TempImag[MinK] = 1.0E200;
  }

 delete[] TempReal;
 delete[] TempImag;
}
//---------------------------------------------------------------------------


// This function is here for reference. It uses Horners Method to compute P(X)
// It will also return P'(X) if DY != NULL
// See:  http://en.wikipedia.org/wiki/Horner%27s_method
long double CalcYandDY(long double *P, int N, long double X, long double *DY)
{
 int j;
 long double Y, dY;
 Y = P[0];
 dY = Y;
 for(j=1; j<=N-1; j++)
  {
   Y  = Y*X + P[j];  // P(X) using Horner's method.
   dY = dY*X + Y;    // P'(X) i.e. The derivative of P at X.
  }
 Y = Y*X + P[N];     // Complete the PT(X) calculation.
 if(DY != NULL)*DY = dY;
 return(Y);
}
//---------------------------------------------------------------------------

// This function is here for reference.
// It divides a numerator polynomial by a denominator polynomial to form a quotient polynomial and remainder polynomial.
// The degree of the numerator and denominator are Nn and Nd respectively.
// The degree of the quotient will be Nn - Nd
// The remainder can be zero, but in general, it's order is one less than the denominator poly.
void PolyDiv(long double *Numer, int Nn, long double *Denom, int Nd, long double *Q, long double* R)
{
 int j, k;
 for (j=0; j<=Nn; j++)
  {
   R[j] = Numer[j];  // Init the remainder to the numerator values.
   Q[j] = 0.0;
  }

 for(k=0; k<=Nn-Nd; k++)
  {
   Q[k] = R[k]/Denom[0];
   for(j=0; j<=Nd; j++)
    {
     R[j+k] -= Q[k] * Denom[j];
    }
  }

 for(j=0; j<Nd; j++)
  {
   R[j] = R[j+Nn-Nd+1];  // Shift the remainder values to the top of the array. The first Nn-Nd+1 values equal zero.
  }

 R[Nd] = 0.0;  // Otherwise the last 2 R terms are the same and is confusing.
}

//---------------------------------------------------------------------------
