//---------------------------------------------------------------------------

#ifndef P51TestCodeH
#define P51TestCodeH
//---------------------------------------------------------------------------

 long double BuildPolyFromRoots(int N, long double *RealRoot, long double *ImagRoot, long double *PolyCoeff);
 void SortRoots(int N, long double *OrigRealRoot, long double *OrigImagRoot, long double *RealRoot, long double *ImagRoot);
 long double CalcYandDY(long double *P, int N, long double X, long double *DY);
 void PolyDiv(long double *Numer, int Nn, long double *Denom, int Nd, long double *Q, long double* R);
#endif
