#ifndef COLOR_CLUSTER_H_
#define COLOR_CLUSTER_H_
#include "qdbmp.h"
#include "convert.h"
typedef struct {
	UCHAR R;
	UCHAR G;
	UCHAR B;
	UINT x;
	UINT y;
} tPixel;
typedef struct {
	tPixel root;
	tPixel *nodes;
} tCluster;

void RandomCluster(BMP* image, tCluster *cluster);
void SelectedCluster(BMP* image, int x, int y, tCluster *cluster);
void setPixel(tPixel *pixel, UCHAR R, UCHAR G, UCHAR B, UINT x, UINT y);
double getLabEuclideanDistance(double CIE_l1, double CIE_a1, double CIE_b1, double CIE_l2, double CIE_a2, double CIE_b2);
double getRGBEuclideanDistance(UCHAR R1, UCHAR G1, UCHAR B1, UCHAR R2, UCHAR G2, UCHAR B2);
void clusterizeRGB(BMP *image, BMP *img_output, tCluster *clusters, int K);
void clusterizeLAB(BMP *image, BMP *img_output, tCluster *clusters, int K, DIFFUSER_REFERENCE diffuser);
#endif
