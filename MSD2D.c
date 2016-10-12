#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef NMAX
#define NMAX 1000000
#endif

int main(int argc, char const *argv[])
{
    if (argc != 3) {
	fprintf(stderr, "Missing arguments.\n");
	fprintf(stderr, "%s pMSD dt\n", argv[0]);
	exit(0);
    }
    int pMSD = atoi(argv[1]);
    float dt = atof(argv[2]);
    float tempX, tempY;
    int N = 0;

    float **pos;
    pos = (float **) malloc(NMAX * sizeof(float *));
    for (int i = 0; i < NMAX; ++i) {
	pos[i] = (float *) malloc(2 * sizeof(float));
    }

    while (scanf("%f %f\n", &tempX, &tempY) != EOF) {
	pos[N][0] = tempX;
	pos[N][1] = tempY;
	N++;
    }

    int nlags = round(N * pMSD / 100);
    float MSDdummy = 0;
    int tick = 0;
    //fprintf(stdout, "Parsee %d posiciones\n", N);
    //fprintf(stdout, "Me quedo con %d lags para en MSD\n",nlags );
    for (int i = 1; i <= nlags; i++) {
	for (int j = 0; j < N - i; j++) {
	    MSDdummy +=
		((pos[j + i][0] - pos[j][0]) * (pos[j + i][0] -
						pos[j][0])) + ((pos[j +
								    i][1] -
								pos[j][1])
							       *
							       (pos[j + i]
								[1] -
								pos[j]
								[1]));
	}
	fprintf(stdout, "%f %f\n", dt * i, MSDdummy / (N - i));
	MSDdummy = 0;
    }
    free(pos);
    return 0;
}
