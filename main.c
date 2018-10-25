#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "headers/qdbmp.h"
#include "headers/convert.h"
#include "headers/color_cluster.h"

int main(int argc, char **argv) {
	UINT width, height;
	BMP* img_input; 
	BMP* img_output;
	tCluster *clusters;

	if(argc < 5) {
		fprintf(stderr, "Usage: colorcluster <numero de clusters> <arquivo entrada> <arquivo saida> -<r/s> -<rgb/lab>\n");
		return -1;
	}

	int k = atoi(argv[1]);
	clusters = calloc(k, sizeof(tCluster));
	img_input = BMP_ReadFile(argv[2]);
	BMP_CHECK_ERROR(stdout, -1);

	width = BMP_GetWidth(img_input);
	height = BMP_GetHeight(img_input);
	img_output = BMP_Create(width, height, 24);
	BMP_CHECK_ERROR(stdout, -2);

	//Chosing between Random points or User input points
	if(argv[4][1] == 'r'){
		for(int i = 0; i < k; i++){ 
			RandomCluster(img_input, &clusters[i]);
			printf("Cluster %d: {%ld,%ld} R=%d, G=%d, B=%d\n", i, clusters[i].root.x, clusters[i].root.y, 
				clusters[i].root.R, clusters[i].root.G, clusters[i].root.B);
		}
	}
	int in_x, in_y;
	if(argv[4][1] == 's'){
		for(int i = 0; i < k; i++){
			printf("Insira as coordenadas do centroide %d:\n", i);
			scanf("%d", &in_x);
			scanf("%d", &in_y);
			SelectedCluster(img_input, in_x, in_y, &clusters[i]);
		}
	}

	//Chosing between sRGB and CIE-lab
	if(argv[5][2] == 'g'){ 
		clusterizeRGB(img_input, img_output, clusters, k);
		for(int i = 0; i < k; i++) {
			printf("Custo total do Cluster %d: %f\n", i, clusters[i].custoTotal);
		}
	}
	/*DIFFUSER_REFERENCE dif;
	if(argv[5][2] == 'a'){
		printf("Escolha uma referencia de difusor:\n" 
			"0-)A\t5-)D65\t10-)F3\t15-)F8\n"
			"1-)B\t6-)D75\t11-)F4\t16-)F9\n"
			"2-)C \t7-)E \t12-)F5\t17-)F10\n"
			"3-)D50\t8-)F1\t13-)F6\t18-)F11\n"
			"4-)D55\t9-)F2\t14-)F7\t19-)F12\n");
		scanf("%d", &dif);
		printf("%d\n", dif);
	     	if(dif >= 0 && dif <= 19) clusterizeLAB(img_input, img_output, clusters, k, dif);
		else{ 
			puts("Entrada invalida."); 
			return -1;
		}
	} else return -1;*/
	

	BMP_WriteFile(img_output, argv[3]);
	BMP_Free(img_input);
	BMP_Free(img_output);
	free(clusters);
	return 0;
}
