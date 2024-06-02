
#pragma once
#include <lcom/lcf.h>




double DISC (double A, double B, double C);

void QDRTC(double A, double B, double C, double *X1, double *Y1, double *X2, double *Y2);

void EVAL(double X, double A, double B, double C, double D, double *Q, double *Qprime, double *B1, double *C2);

void QBC(double A, double B, double C, double D, double *X, double *X1, double *Y1, double *X2, double *Y2);

int quartic(double a, double b, double c, double d, double e, double *ans);

int solveQuadratic(double a, double b, double c, double *ans);

double cubic(double b, double c, double d);

double smallerPositiveQuarticRoot(double a, double b, double c, double d, double e);
