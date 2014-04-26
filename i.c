#include <stdio.h>
#include <omp.h>
#define SIZE 2000	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "help_func.h"

double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];

void unoptimized_triad(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

int main(int argc, const char *argv[])
{
	/*
	double A[SIZE][SIZE] = {1,2,3,4,5,6,7,8,9};
	double B[SIZE][SIZE] = {3,2,3,4,5,6,7,8,9};
	double C[SIZE][SIZE] = {1,10,3,4,5,6,7,8,9};
	*/

	fill_random2d_double_seed(A,42);
	fill_random2d_double_seed(B,42);
	fill_random2d_double_seed(C,42);

	double start_time = omp_get_wtime();
	unoptimized_triad(A,B,C);
	double end_time = omp_get_wtime() - start_time;

	//print_array2d_double('C',C);

	printf("unoptimized base line time: %f\n",end_time);

	double gflops = (2*SIZE/(end_time * 1000000000))*SIZE*SIZE;
	printf("Baseline GFLOP/s = %f\n",gflops);
	return 0;
}
