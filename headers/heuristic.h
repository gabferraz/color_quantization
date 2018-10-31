#ifndef HEURISTIC_H_
#define HEURISTIC_H_
#include "color_cluster.h"
#include "vec.h"
void VND(BMP *image, BMP *img_output, tCluster *clusters, int k, DIFFUSER_REFERENCE diffuser);
void swapCl(BMP *image, BMP *img_output, tCluster *c, int K, DIFFUSER_REFERENCE diffuser);
void insertionCl(BMP *image, BMP *img_output, tCluster *c1, tCluster *c2, int K, DIFFUSER_REFERENCE diffuser);
#endif
