
#include <stdio.h>
#include <omp.h>
#include <smmintrin.h>
#define SIZE 3	/* size of the array */ 
#include <time.h>
#include <stdlib.h>
#include "../../help_func.h"
#include <malloc.h>


#define NUMTHREADS 1

//double A[SIZE][SIZE];
//double B[SIZE][SIZE];
//double C[SIZE][SIZE];

void omp_triad_vec(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE])
{
  register __m128d a,b,c,v;
  //double my_a = 0;
  double my_a = 0;
  double my_b = 0;
  double my_mult = 0;
	//#pragma omp parallel for

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
		   for (int k = 0; k < SIZE; k++) {
			   a = _mm_load_pd(&A[i][k]);
			   //a = _mm_load_pd(&A[i][k]);
			   b = _mm_load_pd(&B[k][j]);
			   //b = _mm_load_pd(&B[k][j]);
			   c = _mm_load_pd(&C[i][j]);
			   //c = _mm_load_pd(&C[i][j]);
			   printf("my_b %f\n",B[k][j]);
			   printf("my_a %f\n",A[i][k]);
			   printf("my_c %f\n",C[i][j]);

			   //v = _mm_set_pd(0,0);
			   v  = _mm_mul_pd1(a,b);
			   _mm_store_pd(&my_mult,v);
			   printf("my_mult %f\n",my_mult);
			   
			   //v  =_mm_add_pd( _mm_mul_pd(a,b),c);
			   //_mm_store_pd(&my_mult,v);
			   //printf("my_mult %f\n",my_mult);
			   //printf("i,j,k %d %d %d\n",i,j,k);
			   //_mm_store_pd(&C[i][j],c);
			}
		}
	}
}

int main(int argc, const char *argv[])
{
	omp_set_num_threads(NUMTHREADS);
	
	size_t bytes = SIZE*SIZE*8;
	double A[SIZE][SIZE] = _aligned_malloc(bytes,8);
	double A[SIZE][SIZE] = {1,2,3,4,5,6,7,8,9};
	double B[SIZE][SIZE] = {3,2,3,4,5,6,7,8,9};
	double C[SIZE][SIZE] = {1,10,3,4,5,6,7,8,9};
	/*
	fill_random2d_double_seed(A,42);
	fill_random2d_double_seed(B,42);
	fill_random2d_double_seed(C,42);
	*/
	double start_time = omp_get_wtime();
	omp_triad_vec(A,B,C);
	double end_time = omp_get_wtime() - start_time;

	//print_array2d_double('C',C);

	printf("unoptimized omp time: %f\n",end_time);
	double gflops = 2.*SIZE*SIZE*SIZE/(end_time * 1000000000.);
	printf("Unomptimized OMP GFLOP/s = %f\n",gflops);


	/*
	#pragma omp parallel
	{
		printf("omp_get_num_threads=%d\n",omp_get_num_threads());
	}
	*/

	return 0;
}
