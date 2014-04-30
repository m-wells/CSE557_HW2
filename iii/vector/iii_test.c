#include <stdio.h>
#include <omp.h>
#include <smmintrin.h>
#define SIZE 20 /* size of the array */
#include <time.h>
#include <stdlib.h>
#include "../../help_func.h"


#define NUMTHREADS 1


#include <ia32intrin.h> 
 
// n a multiple of 4, x is 16-byte aligned 
void addindex_vec(float *x, int n) { 
  __m128 index, incr, x_vec; 
 
  index = _mm_set_ps(0, 1, 2, 3); 
  incr = _mm_set1_ps(4); 
  for (int i = 0; i < n/4; i++) { 
    x_vec = _mm_load_ps(x+i*4); // load 4 floats 
    x_vec = _mm_add_ps(x_vec, index); // add index 
    _mm_store_ps(x+i*4, x_vec); // store back 
    index = _mm_add_ps(index, incr); // increment index 
  } 
  // printf("%d\n",x); 
} 

int main()
{
  omp_set_num_threads(NUMTHREADS);
  /*                                                                                                                             
        double A[SIZE][SIZE] = {1,2,3,4,5,6,7,8,9};                                                                                    
        double B[SIZE][SIZE] = {3,2,3,4,5,6,7,8,9};                                                                                    
        double C[SIZE][SIZE] = {1,10,3,4,5,6,7,8,9};                                                                                   
  */

  float LER = [2,4,6,8,10,12,14,16,18,20,22,24];
  addindex_vec(LER,12);

}
