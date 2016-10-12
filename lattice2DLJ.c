#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <configini.h>

#define NGRID 10000

double distance(double,double,double,double);
double energy(double,double);

int main(int argc, char const *argv[]) {

	if (argc < 2)
	{
		printf("Missing config file.\n");
		exit(0);
	}

	srand(time(NULL));

	FILE *fd;
	gsl_rng *rng = gsl_rng_alloc (gsl_rng_taus);
	gsl_rng_set(rng,rand());
	double density, sig0, sig1, kT, r0;
	int N;
	bool printGrid;

	Config *cfg = NULL;
	ConfigReadFile(argv[1],&cfg);
	ConfigReadDouble(cfg,"lattice","density",&density,0.25);
	ConfigReadDouble(cfg,"lattice","sig0",&sig0,0.1);
	ConfigReadDouble(cfg,"lattice","sig1",&sig1,1.0);
	ConfigReadDouble(cfg,"lattice","kT",&kT,0.1);
	ConfigReadDouble(cfg,"lattice","r0",&r0,0.2);
	ConfigReadInt(cfg,"lattice","N",&N,10000);
	ConfigReadBool(cfg,"lattice","print_grid",&printGrid,false);
	ConfigFree(cfg);

	double sig = 0.5 * (sig0 + sig1);
	double cutoff = 10.5 * sig;	// interaction cutoff
	double d = 1/sqrt(density); // grid separation
	double Upre = 0, Upost = 0;
	double x = 0.0;
	double y = 0.0;
	double newx, newy;
	double rPre, rPost, dU, p, pMove;

	if (printGrid) { fd = fopen("grid.txt","w"); }

	double **grid;
	grid = (double**)malloc(NGRID*sizeof(double*));
	for (int i = 0; i < NGRID; i++)	{
		grid[i] = (double*)malloc(2*sizeof(double));
	}

	int k = 0;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			grid[k][0] = (i - 49.5) * d;
			grid[k][1] = (j - 49.5) * d;
			if (printGrid) { fprintf(fd, "%f %f\n", grid[k][0], grid[k][1]); }
			k++;
		}
	}

	if (printGrid) { fclose (fd); }

	for (int i = 0; i < N; i++)	{
		newx = x + gsl_ran_gaussian(rng,r0);
		newy = y + gsl_ran_gaussian(rng,r0);
		for (int j = 0; j < NGRID; j++)	{
			rPre = distance(x,y,grid[j][0],grid[j][1]);
			rPost = distance(newx,newy,grid[j][0],grid[j][1]);
			if ( rPre < cutoff ) {
				Upre +=	energy(rPre,sig);
				Upost += energy(rPost,sig);
			}
		}
		dU = Upost - Upre;
		
		if ( dU < 0 ) {
			x = newx;
			y = newy;
		} else {
			p = gsl_ran_flat(rng,0,1);
			pMove = exp(-dU/kT);
			if ( pMove > p ) {
				x = newx;
				y = newy;
			}
		}
		//printf("Estaba en %f %f y moví a %f %f, dU = %f, pMove = %f, p = %f\n",x,y,newx,newy,dU,pMove,p);
		printf("%f %f\n",x,y );
		Upre = 0;
		Upost = 0;

	}
}

double distance(double x1, double y1, double x0, double y0) {
	return sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
}

double energy(double dr, double sig) {
	return pow(sig/dr,12) - pow(sig/dr,6);
}
