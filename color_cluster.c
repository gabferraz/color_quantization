#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "headers/color_cluster.h"

void RandomCluster(BMP* image, tCluster *cluster){
	int size_x = BMP_GetWidth(image);
	int size_y = BMP_GetHeight(image);

	cluster->root.x = random() % size_x;
	cluster->root.y = random() % size_y;

	UCHAR R, G, B;
	BMP_GetPixelRGB(image, cluster->root.x, cluster->root.y, &R, &G, &B);

	cluster->root.R = R;
	cluster->root.G = G;
	cluster->root.B = B;
	cluster->nodes = NULL;
	cluster->custoTotal = 0;
}

void SelectedCluster(BMP* image, int x, int y, tCluster *cluster){
        cluster->root.x = x;
	cluster->root.y = y;
	
	UCHAR R, G, B;
        BMP_GetPixelRGB(image, x, y, &R, &G, &B);

        cluster->root.R = R;
        cluster->root.G = G;
        cluster->root.B = B;
	cluster->nodes = NULL;
}

void setPixel(tPixel *pixel, UCHAR R, UCHAR G, UCHAR B, UINT x, UINT y, double distancia){
	pixel->R = R;
	pixel->G = G;
	pixel->B = B;
	pixel->x = x;
	pixel->y = y;
	pixel->distancia = distancia;
}

double getLabEuclideanDistance(double CIE_l1, double CIE_a1, double CIE_b1, double CIE_l2, double CIE_a2, double CIE_b2){
	double deltaL = pow(CIE_l1 - CIE_l2, 2);	
	double deltaA = pow(CIE_a1 - CIE_a2, 2);	
	double deltaB = pow(CIE_b1 - CIE_b2, 2);

	return sqrt(deltaL + deltaA + deltaB);	
}

double getRGBEuclideanDistance(UCHAR R1, UCHAR G1, UCHAR B1, UCHAR R2, UCHAR G2, UCHAR B2){
	double deltaR = pow(R1 - R2, 2);	
	double deltaG = pow(G1 - G2, 2);	
	double deltaB = pow(B1 - B2, 2);

	return sqrt(deltaR + deltaG + deltaB);	
}

void clusterizeRGB(BMP *image, BMP *img_output, tCluster *clusters, int K){
	int size_x = BMP_GetWidth(image);
        int size_y = BMP_GetHeight(image);
	int min_dist, last_cluster, cluster_flag = 0;
	UCHAR r, g, b;

	for(int x = 0; x < size_x; x++) {
		for(int y = 0; y < size_y; y++) {
			min_dist = INT_MAX;
			cluster_flag = 0;
			BMP_GetPixelRGB(image, x, y, &r, &g, &b);
			
			for(int i = 0; i < K; i++) {
				if(x == clusters[i].root.x && y == clusters[i].root.y) cluster_flag = 1;
				break;
			}
			if(cluster_flag == 1) {
				BMP_SetPixelRGB(img_output, x, y, r, g, b);
				break;
			}

			for(int d = 0; d < K; d++) {
				double dist = getRGBEuclideanDistance(clusters[d].root.R, clusters[d].root.G, clusters[d].root.B, r, g, b);
				if(dist < min_dist){ 
					min_dist = dist;
					last_cluster = d;
				}
			}
			if(clusters[last_cluster].nodes == NULL) {
				clusters[last_cluster].nodes = (tPixel*) malloc(sizeof(tPixel));
				setPixel(clusters[last_cluster].nodes, r, g, b, x, y, min_dist);
				clusters[last_cluster].custoTotal += min_dist;
			} else {
				tPixel *tmp;
				if((tmp = realloc(clusters[last_cluster].nodes, 
				    sizeof(clusters[last_cluster].nodes) + sizeof(tPixel))) == NULL){
					fprintf(stderr, "ERROR: Realloc failed.");
				}
				clusters[last_cluster].nodes = tmp;
                                setPixel(clusters[last_cluster].nodes, r, g, b, x, y, min_dist);
				clusters[last_cluster].custoTotal += min_dist;
			}
			BMP_SetPixelRGB(img_output, x, y, clusters[last_cluster].root.R, clusters[last_cluster].root.G, clusters[last_cluster].root.B);
		}
	}
}

void clusterizeLAB(BMP *image, BMP *img_output, tCluster *clusters, int K, DIFFUSER_REFERENCE diffuser){
        int size_x = BMP_GetWidth(image);
        int size_y = BMP_GetHeight(image);
	int min_dist, last_cluster, cluster_flag = 0;
	UCHAR r, g, b;
	double X, Y, Z;
	double clusterX, clusterY, clusterZ;
	double CIE_l, CIE_a, CIE_b;
	double clusterL, clusterA, clusterB;

	for(int x = 0; x < size_x; x++) {
		for(int y = 0; y < size_y; y++) {
			min_dist = INT_MAX;
			cluster_flag = 0;
			BMP_GetPixelRGB(image, x, y, &r, &g, &b);
			
			for(int i = 0; i < K; i++) {
				if(x == clusters[i].root.x && y == clusters[i].root.y) cluster_flag = 1;
				break;
			}
			if(cluster_flag == 1) {
				BMP_SetPixelRGB(img_output, x, y, r, g, b);
				break;
			}

			for(int d = 0; d < K; d++) {
				sRGB2xyz(clusters[d].root.R, clusters[d].root.G, clusters[d].root.B, &clusterX, &clusterY, &clusterZ);
				xyz2LAB(clusterX, clusterY, clusterZ, &clusterL, &clusterA, &clusterB, diffuser);
				double dist = getLabEuclideanDistance(clusterL, clusterA, clusterB, CIE_l, CIE_a, CIE_b);
				if(dist < min_dist){ 
					min_dist = dist;
					last_cluster = d;
				}
			}

			if(clusters[last_cluster].nodes == NULL) {
				clusters[last_cluster].nodes = (tPixel*) malloc(sizeof(tPixel));
				setPixel(clusters[last_cluster].nodes, r, g, b, x, y, min_dist);
			} else {
				tPixel *tmp;
				if((tmp = realloc(clusters[last_cluster].nodes, sizeof(clusters[last_cluster].nodes) + sizeof(tPixel))) == NULL)
					fprintf(stderr, "ERROR: Realloc failed.");
				clusters[last_cluster].nodes = tmp;
                                setPixel(clusters[last_cluster].nodes, r, g, b, x, y, min_dist);
			}
			BMP_SetPixelRGB(img_output, x, y, clusters[last_cluster].root.R, clusters[last_cluster].root.G, 
					clusters[last_cluster].root.B);
		}
	}
}
