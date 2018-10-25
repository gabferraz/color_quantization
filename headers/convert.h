#ifndef CONVERT_H_
#define CONVERT_H_
#include "qdbmp.h"

typedef enum {
	A,
	B,
	C,
	D50,
	D55,
	D65,
	D75,
	E,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12
} DIFFUSER_REFERENCE;

void sRGB2xyz(UCHAR sR, UCHAR sG, UCHAR sB, double *X, double *Y, double *Z);
void xyz2sRGB(UCHAR *sR, UCHAR *sG, UCHAR *sB, double X, double Y, double Z);
void xyz2LAB(double X, double Y, double Z, double *CIE_l, double *CIE_a, double *CIE_b, DIFFUSER_REFERENCE diffuser);
void LAB2xyz(double *X, double *Y, double *Z, double CIE_l, double CIE_a, double CIE_b, DIFFUSER_REFERENCE diffuser);
#endif
