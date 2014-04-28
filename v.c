#include <stdio.h>
#include <omp.h>
#define SIZE 2000	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "help_func.h"

#include "mkl.h"

//double A[SIZE][SIZE];
//double B[SIZE][SIZE];
//double C[SIZE][SIZE];

/*
void unoptimized_triad_(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}
*/
//void Dgemm_multiply(double a[SIZE][SIZE],double b[SIZE][SIZE],double c[SIZE][SIZE], int N)
//{	
//
//	double alpha = 1.0, beta = 1.0;
//	int incx = 1;
//	int incy = N;
//	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,alpha,b,N,a,N,beta,c,N);
//	//dgemm_(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC)
//	//dgemm('N','N',SIZE,SIZE,SIZE,1.0,&A,SIZE,&B,SIZE,1.0,&C,SIZE);
//}
//


//print array to std out
void print_arr(int N, char * name, double* array)
{	
	int i,j;	
	printf("\n%s\n",name);
	for (i=0;i<N;i++){
		for (j=0;j<N;j++) {
			printf("%g\t",array[N*i+j]);
		}
		printf("\n");
	}
}

//initialize array with random data
void init_arr(int N, double* a)
{	
	int i,j;
	for (i=0; i< N;i++) {
		for (j=0; j<N;j++) {
			a[i*N+j] = ((double)rand()/(double)RAND_MAX);
		}
	}
}


int main(int argc, const char *argv[])
{
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
	int incx = 1;
	int incy = N;

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
