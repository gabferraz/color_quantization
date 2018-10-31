#include <math.h>
#include "headers/convert.h"

//Set the reference values of a reflecting diffuser.
void getReference(double *refX, double *refY, double *refZ, DIFFUSER_REFERENCE diffuser){
	*refY = 100;

	switch (diffuser) {
		case (A):
			*refX = 109.85;
			*refZ = 35.585;
			break;
		case (B):
			*refX = 99.0927;
			*refZ = 85.313;
			break;
		case (C):
			*refX = 98.074;
			*refZ = 118.232;
			break;
		case (D50):
			*refX = 96.422;
			*refZ = 82.521;
			break;
		case (D55):
			*refX = 95.682;
			*refZ = 92.149;
			break;
		case (D65):
			*refX = 95.047;
			*refZ = 108.883;
			break;
		case (D75):
			*refX = 94.972;
			*refZ = 122.638;
			break;
		case (E):
			*refX = 100;
			*refZ = 100;
			break;
		case (F1):
			*refX = 92.834;
			*refZ = 103.665;
			break;
		case (F2):
			*refX = 99.187;
			*refZ = 67.395;
			break;
		case (F3):
			*refX = 103.754;
			*refZ = 49.861;
			break;
		case (F4):
			*refX = 109.147;
			*refZ = 38.813;
			break;
		case (F5):
			*refX = 90.872;
			*refZ = 98.723;
			break;
		case (F6):
			*refX = 97.309;
			*refZ = 60.191;
			break;
		case (F7):
			*refX = 95.044;
			*refZ = 108.755;
			break;
		case (F8):
			*refX = 96.413;
			*refZ = 82.333;
			break;
		case (F9):
			*refX = 100.365;
			*refZ = 67.868;
			break;
		case (F10):
			*refX = 96.174;
			*refZ = 81.712;
			break;
		case (F11):
			*refX = 100.966;
			*refZ = 64.370;
			break;
		case (F12):
			*refX = 108.046;
			*refZ = 39.228;
			break;
	}

}

//Convert sRGB color scheme to XYZ
void sRGB2xyz(UCHAR sR, UCHAR sG, UCHAR sB, double *X, double *Y, double *Z){
	double R = (double)sR/255;
	double G = (double)sG/255;
	double B = (double)sB/255;

	if (R > 0.04045) R = pow((R+0.055) / 1.055, 2.4);
	else R = R / 12.92;

	if (G > 0.04045) G = pow((G+0.055) / 1.055, 2.4);
	else G = G / 12.92;
	
	if (B > 0.04045) B = pow((B+0.055) / 1.055, 2.4);
	else B = B / 12.92;

	R *= (double)100;
	G *= (double)100;
	B *= (double)100;
	//printf("%f %f %f\n",R,G,B );

	*X = (double) R*0.4124 + (double) G*0.3576 + (double) B*0.1805;
	*Y = (double) R*0.2126 + (double) G*0.7152 + (double) B*0.0722;
	*Z = (double) R*0.0193 + (double) G*0.1192 + (double) B*0.9505;
	//printf("X Y Z %f %f %f\n",X,Y,Z );

}

//Convert XYZ color scheme to sRGB
void xyz2sRGB(UCHAR *sR, UCHAR *sG, UCHAR *sB, double X, double Y, double Z){
	double x = X / 100;
	double y = Y / 100;
	double z = Z / 100;

	double R = x*3.2406 + y*-1.5372 + z*-0.4986;
	double G = x*-0.9689 + y*1.8758 + z*0.0415;
	double B = x*0.0557 + y*-0.2040 + z*1.0570;
	
	if (R > 0.0031308) R = 1.055 * pow(R, 1/2.4);
	else R = R * 12.92;

	if (G > 0.0031308) G = 1.055 * pow(G, 1/2.4);
	else G = G * 12.92;
	
	if (B > 0.0031308) B = 1.055 * pow(B, 1/2.4);
        else B = B * 12.92;

	*sR = (UCHAR) R*255;
	*sG = (UCHAR) G*255;
	*sB = (UCHAR) B*255;
}

//Convert XYZ color scheme to CIE-L*ab
void xyz2LAB(double X, double Y, double Z, double *CIE_l, double *CIE_a, double *CIE_b, DIFFUSER_REFERENCE diffuser){
	double refX, refY, refZ;
	getReference(&refX, &refY, &refZ, diffuser);

	double x = (double) X/refX;
	double y = (double) Y/refY;
	double z = (double) Z/refZ;

        if (x > 0.008856) x = pow(x, (double)1/3);
        else x = (7.787*x) + (16/116);

        if (y > 0.008856) y = pow(y, (double)1/3);
        else y = (7.787*y) + (16/116);

        if (z > 0.008856) z = pow(z, (double)1/3);
        else z = (7.787*z) + (16/116);

	*CIE_l = (116*y) - 16;
	*CIE_a = 500 * (x - y);
	*CIE_b = 200 * (y - z);
}

//Convert CIE-L*ab color scheme to XYZ
void LAB2xyz(double *X, double *Y, double *Z, double CIE_l, double CIE_a, double CIE_b, DIFFUSER_REFERENCE diffuser){
	double refX, refY, refZ;
	getReference(&refX, &refY, &refZ, diffuser);

	double y = (CIE_l+16) / 116;
	double x = CIE_a / (500+y);
	double z = y - (CIE_b/200);

        if (pow(x,3) > 0.008856) x = pow(x, 3);
        else x = (x-(16/116))/7.787;

        if (pow(y,3) > 0.008856) y = pow(y, 3);
        else y = (y-(16/116))/7.787;

        if (pow(z,3) > 0.008856) z = pow(z, 3);
        else z = (z-(16/116))/7.787;

	*X = x*refX;
	*Y = y*refY;
	*Z = z*refZ;
}
