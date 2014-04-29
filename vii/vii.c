#include <stdio.h>
#include <omp.h>
#define SIZE 9	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "../help_func.h"

#include <mpi.h>
#include "mkl.h"


//A part of this program is based on Intel's cblas_dgemm example: seen here 
//https://software.intel.com/sites/default/files/article/171460/mkl-lab-solution.c


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

	//print_arr(N,"A",A);
	//print_arr(N,"B",B);
	//print_arr(N,"C",C);
	double start_time = omp_get_wtime();

	int ione = 1;
	int descArr[9]; //Descriptor array
	int row_block_size = 3;
	int col_block_size = 3;
	int izero = 0;
	int contxt =
	int maxmp =
	int info = 0;
	int block_size = 3;
	int my_row = 0;
	int num_proc_row;
	//NUMROC
	np = numroc_(N, row_block_size, my_row, izero, num_proc_row );
	maxnp = imax(1,np);
	//
	descinit_(descArr, N, N, row_block_size, column_block_size, izero, izero, 0 ,col_block_size,info);
	//cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,alpha,A,ione,ione,descA,
	//		N,B,N,beta,C,N);
	pdgemm_('N','N',N,N,N,alpha,A,N,B,N,beta,C,N);
	//unoptimized_triad(A,B,C);
	double end_time = omp_get_wtime() - start_time;

	//print_arr(N,"C",C);

	printf("unoptimized base line time: %f\n",end_time);

	double gflops = (2*SIZE/(end_time * 1000000000))*SIZE*SIZE;
	printf("Baseline GFLOP/s = %f\n",gflops);

	return 0;
}
