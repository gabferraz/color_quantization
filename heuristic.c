#include "headers/heuristic.h"
#include <stdlib.h>
#include <stdio.h>

void swapCl(BMP *image, BMP *img_output, tCluster *c, int K, DIFFUSER_REFERENCE diffuser){
	tPixel *tmp1, *tmp2;
	tmp1 = malloc(sizeof(tPixel));
	tmp1 = &(c->root);
	tmp2 = malloc(sizeof(tPixel));
	tmp2 = vec_last(&(c->nodes));
	setPixel(&(c->root), tmp2->R, tmp2->G, tmp2->B, tmp2->x, tmp2->y, 0);
	vec_pop(&(c->nodes));
	vec_push(&(c->nodes),tmp1);
	//clusterizeLAB(image, img_output, c, K, diffuser);
}

void insertionCl(BMP *image, BMP *img_output, tCluster *c1, tCluster *c2, int K, DIFFUSER_REFERENCE diffuser){
	UCHAR r, g, b;
	double X, Y, Z;
	double tempX, tempY, tempZ;
	double CIE_l, CIE_a, CIE_b;
	double clusterL, clusterA, clusterB;

	tPixel *tmp1, *tmp2;
	tmp1 = malloc(sizeof(tPixel));
	tmp1 = &(c1->root);
	tmp2 = malloc(sizeof(tPixel));
	tmp2 = vec_last(&(c1->nodes));
	setPixel(&(c1->root), tmp2->R, tmp2->G, tmp2->B, tmp2->x, tmp2->y, 0);
	vec_pop(&(c1->nodes));

	sRGB2xyz(c2->root.R, c2->root.G, c2->root.B, &tempX, &tempY, &tempZ);
	xyz2LAB(tempX, tempY, tempZ, &clusterL, &clusterA, &clusterB, diffuser);
	//Converter cores do pixel
	sRGB2xyz(tmp1->R, tmp1->G, tmp1->B, &tempX, &tempY, &tempZ);
	xyz2LAB(tempX, tempY, tempZ, &CIE_l, &CIE_a, &CIE_b, diffuser);
	double dist = getLabEuclideanDistance(clusterL, clusterA, clusterB, CIE_l, CIE_a, CIE_b);
	tmp1->distancia = dist;
	vec_push(&(c2->nodes), tmp1);
}

double somaCusto (tCluster *c, int k){
	double soma = 0;
	for (int i = 0; i < k; i++){
		soma = soma + c[i].custoTotal;
	}
	return soma;
}

void VND(BMP *image, BMP *img_output, tCluster *clusters, int k, DIFFUSER_REFERENCE diffuser){
	int q = 1, i = 0, somaCustoAntigo, somaCustoNovo;
  	tCluster *tmpClusters = clusters;
  	somaCustoAntigo = somaCusto(clusters, k);
  	while(i < k-1) {
    	if(q == 1) {
      		swapCl(image, img_output, &(tmpClusters[i]), k, diffuser);
      		clusterizeLAB(image, img_output, tmpClusters, k, diffuser);
      		printf("Custo antigo: %d\n", somaCustoAntigo);
      		somaCustoNovo = somaCusto(tmpClusters, k);
      		printf("Custo novo: %d\n", somaCustoNovo);
      	if(somaCustoNovo < somaCustoAntigo) {
        	clusters = tmpClusters;
        	somaCustoAntigo = somaCustoNovo;
        	q = 1;
      	} else {
        	q++;
      }
    }
    if(q == 2) {
      insertionCl(image, img_output, &(tmpClusters[i]), &(tmpClusters[i + 1]), k, diffuser);
      clusterizeLAB(image, img_output, tmpClusters, k, diffuser);
      printf("Custo antigo: %d\n", somaCustoAntigo);
      somaCustoNovo = somaCusto(tmpClusters, k);
      printf("Custo novo: %d\n", somaCustoNovo);
      if(somaCustoNovo < somaCustoAntigo) {
        clusters = tmpClusters;
        somaCustoAntigo = somaCustoNovo;
        q = 1;
      } else {
        q++;
      }
    } else {
      		i++;
      		q = 1;
    	}
	}
	printf("Custo antigo: %d\n", somaCustoAntigo);
    printf("Custo novo: %d\n", somaCustoNovo);
}

