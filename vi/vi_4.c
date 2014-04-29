#include <stdio.h>
#include <omp.h>
#define SIZE 8000	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "../help_func.h"

#include "mkl.h"

#define NUMTHREADS 4

//A part of this program is based on Intel's cblas_dgemm example: seen here 
//program is based on 


int main(int argc, const char *argv[])
{
	omp_set_num_threads(NUMTHREADS);
	//double* A = {1,2,3,4,5,6,7,8,9};
	//double* B = {3,2,3,4,5,6,7,8,9};
	//double* C = {1,10,3,4,5,6,7,8,9};

	double* A;
	double* B;
	double* C;
	int N = SIZE;
	A=(double*) malloc( sizeof(double)*N*N );
	B=(double*) malloc( sizeof(double)*N*N );
	C=(double*) malloc( sizeof(double)*N*N );

	//A = (double[SIZE*SIZE]){1,2,3,4,5,6,7,8,9};
	//B = (double[SIZE*SIZE]){3,2,3,4,5,6,7,8,9};
	//C = (double[SIZE*SIZE]){1,10,3,4,5,6,7,8,9};
	init_arr(N, A);
	init_arr(N, B);
	init_arr(N, C);

	/*
	fill_random2d_double_seed(A,42);
	fill_random2d_double_seed(B,42);
	fill_random2d_double_seed(C,42);
	*/
	double alpha = 1.0, beta = 1.0;

	//print_arr(N,"A",A);
	//print_arr(N,"B",B);
	//print_arr(N,"C",C);
	double start_time = omp_get_wtime();

	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,alpha,A,N,B,N,beta,C,N);
	//unoptimized_triad(A,B,C);
	double end_time = omp_get_wtime() - start_time;

	//print_arr(N,"C",C);

	printf("unoptimized base line time: %f\n",end_time);

	double gflops = (2*SIZE/(end_time * 1000000000))*SIZE*SIZE;
	printf("Baseline GFLOP/s = %f\n",gflops);

	return 0;
}
