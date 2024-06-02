

#include "equations.h"
#include "utilities.h"
#include <math.h>
#include "QuadRootsRevJ.h"

double DISC(double A, double B, double C) {

  printf("INside disc: \n");
  printFloat(A);
  printFloat(B);
  printFloat(C);
  if (A * B > 0) {
    A = fabs(A);
    C = fabs(C);

    while (true) {
      if (A < C) {
        double temp = A;
        A = C;
        C = temp;
      }
      // now 0 < c <= a
      int n = (int) round((double) B / (double) C); // |n - b/c| < 1/2
      if (n != 0) {
        double alpha = A - n * B; // exact if alpha >= -a
        if (alpha >= -A) {
          B -= n * C; // |b| <= c/2
          A = alpha - n * B;
          if (A > 0) {
            continue;
          }
        }
      }
      break;
    }
  }
  double w = C * A;
  double e = fma(-C, A, w);
  double f = fma(B, B, -w);
  double r = f + e;
  return r;
}

void QDRTC(double A, double B, double C, double *X1, double *Y1, double *X2, double *Y2) {
  double b = -B / 2.0;
  printf("INside Quadratic\n");
  printFloat(A);
  printFloat(B);
  printFloat(C);
  printf("\n");
  double q = DISC(A, b, C);

  if (q < 0) {
    *X1 = b / A;
    *X2 = *X1;
    *Y1 = sqrt(-q) / A;
    *Y2 = -*Y1;
  }
  else {
    *Y1 = 0;
    *Y2 = 0;
    double r = b + copysign(sqrt(q), b); // copysign(b) * sqrt(q)
    if (r == 0) {
      *X1 = C / A;
      *X2 = -*X1;
    }
    else {
      *X1 = C / r;
      *X2 = r / A;
    }
  }
}

void EVAL(double X, double A, double B, double C, double D, double *Q, double *Qprime, double *B1, double *C2) {
  double q0 = A * X;
  *B1 = q0 + B;
  *C2 = (*B1) * X + C;
  *Qprime = (q0 + *B1) * X + *C2;
  *Q = (*C2) * X + D;
}

void QBC(double A, double B, double C, double D, double *X, double *X1, double *Y1, double *X2, double *Y2) {
  printf("Inputs: \n");
  printFloat(A);
  printFloat(B);
  printFloat(C);
  printFloat(D);
  double b1, c2, q, qprime;
  double t, r, s, x0;

  if (A == 0) {
    *X = INFINITY;
    A = B;
    b1 = C;
    c2 = D;
    goto fin;
  }
  if (D == 0) {
    *X = 0;
    b1 = B;
    c2 = C;
    goto fin;
  }

  *X = -(B / A) / 3.0;
  EVAL(*X, A, B, C, D, &q, &qprime, &b1, &c2);
  t = q / A;
  r = cbrt(fabs(t));
  s = copysign(1.0, t);

  t = -qprime / A;
  if (t > 0)
    r = 1.324718 * fmax(r, sqrt(t));
  x0 = *X - s * r;
  if (x0 == *X)
    goto fin;

  do {
    *X = x0;
    EVAL(*X, A, B, C, D, &q, &qprime, &b1, &c2);
    if (qprime == 0)
      x0 = *X;
    else
      x0 = *X - (q / qprime) / 1.000000001;
  } while (s * x0 <= s * *X);

  if (fabs(A) * (*X) * (*X) > fabs(D / *X)) {
    c2 = -D / *X;
    b1 = (c2 - C) / *X;
  }

fin:
  printf("Before calling qudratic\n");
  printFloat(A);
  printFloat(b1);
  printFloat(c2);
  printf("\n");

  QDRTC(A, b1, c2, X1, Y1, X2, Y2);
}

int quartic(double a, double b, double c, double d, double e, double *ans) {
  printf("Starting quartic!!!\n");
  b = b / a;
  c = c / a;
  d = d / a;
  e = e / a;

  double p = c - 0.375 * b * b;
  double q = 0.125 * b * b * b - 0.5 * b * c + d;
  double X, X1, Y1, X2, Y2;
    double newD = -0.125 * q * q;
    if (newD < 1e-11){
        printf("Really small indeed\n");
        newD = 0;
    }
  QBC(1, p, 0.25 * p * p + 0.01171875 * b * b * b * b - e + 0.25 * b * d - 0.0625 * b * b * c, newD, &X, &X1, &Y1, &X2, &Y2);
  printf("Finished QBC\n");
  double m = X;
  m = (Y1 == 0 && X1 > 0) ? MIN(X, X1) : X;
  m = (Y2 == 0 && X2 > 0) ? MIN(X, X2) : X;

  if (q == 0.0) {
    if (m < 0.0) {
      return 0;
    }
    int nroots = 0;
    double sqrt_2m = sqrt(2.0 * m);
    if (-m - p > 0.0) {
      double delta = sqrt(2.0 * (-m - p));
      ans[nroots++] = -0.25 * b + 0.5 * (sqrt_2m - delta);
      ans[nroots++] = -0.25 * b - 0.5 * (sqrt_2m - delta);
      ans[nroots++] = -0.25 * b + 0.5 * (sqrt_2m + delta);
      ans[nroots++] = -0.25 * b - 0.5 * (sqrt_2m + delta);
    }

    if (-m - p == 0.0) {
      ans[nroots++] = -0.25 * b - 0.5 * sqrt_2m;
      ans[nroots++] = -0.25 * b + 0.5 * sqrt_2m;
    }

    return nroots;
  }

  if (m < 0.0)
    return 0;
  double sqrt_2m = sqrt(2.0 * m);
  int nroots = 0;
  if (-m - p + q / sqrt_2m >= 0.0) {
    double delta = sqrt(2.0 * (-m - p + q / sqrt_2m));
    ans[nroots++] = 0.5 * (-sqrt_2m + delta) - 0.25 * b;
    ans[nroots++] = 0.5 * (-sqrt_2m - delta) - 0.25 * b;
  }

  if (-m - p - q / sqrt_2m >= 0.0) {
    double delta = sqrt(2.0 * (-m - p - q / sqrt_2m));
    ans[nroots++] = 0.5 * (sqrt_2m + delta) - 0.25 * b;
    ans[nroots++] = 0.5 * (sqrt_2m - delta) - 0.25 * b;
  }
  return nroots;
}

double cubic(double b, double c, double d) {
  double p = c - b * b / 3.0;
  double q = 2.0 * b * b * b / 27.0 - b * c / 3.0 + d;

  if (p == 0.0)
    return pow(q, 1.0 / 3.0);
  if (q == 0.0)
    return 0.0;

  double t = sqrt(fabs(p) / 3.0);
  double g = 1.5 * q / (p * t);
  if (p > 0.0)
    return -2.0 * t * sinh(asinh(g) / 3.0) - b / 3.0;

  if (4.0 * p * p * p + 27.0 * q * q < 0.0)
    return 2.0 * t * cos(acos(g) / 3.0) - b / 3.0;

  if (q > 0.0)
    return -2.0 * t * cosh(acosh(-g) / 3.0) - b / 3.0;

  return 2.0 * t * cosh(acosh(g) / 3.0) - b / 3.0;
}

int solveQuadratic(double a, double b, double c, double *ans) {

  if (a == 0) {
    double u = -c / b;
    *ans = u;
    return 1;
  }

  double d = b * b - (4 * a * c);
  if (d < 0)
    return 0;
  if (d == 0) {
    *ans = -b / (2 * a);
    return 1;
  }

  double sqrtD = sqrt(d);
  ans[0] = (-b + sqrtD) / (2 * a);
  ans[1] = (-b - sqrtD) / (2 * a);
  return 2;
}

double smallerPositiveQuarticRoot(double a, double b, double c, double d, double e) {
  long double realResults[4];
  long double imgResults[4];
  long double coef[5] = {a, b, c, d, e};
  double bestResult = INFINITY;

  int size = QuadCubicRoots(coef, 4, realResults, imgResults);
  for (int i = 0; i < size; i++) {
    if (realResults[i] > 0 && realResults[i] < bestResult && imgResults[i] == 0) {
      bestResult = realResults[i];
    }
  }
  return bestResult;
}
