#include <stdio.h> /* for input/output */
#include <time.h>
#include <stdlib.h>


#define SIZE 10     /* size of the array */

#include "help_func.h"


double b[SIZE][SIZE];



//void print_array(char ch, int a[SIZE][SIZE], int n);
int main() {

   /* initialize a using {} initializer */
   //int a[SIZE][SIZE] = {{ 0,  1,  2,  3},
   //                     {10, 11, 12, 13},
   //                     {20, 21, 22, 23},
   //                     {30, 31, 32, 33}};
   //int b[SIZE][SIZE]; /* declare second array */
   //int c[SIZE][SIZE]; /* declare third array */
   int i = 0, j = 0, k = 0; /* for loops */


   printf("%d", RAND_MAX);
   srand((unsigned)time(NULL));
   //print_array('a', a, SIZE);

   /* initialize b with code */
   //for (i = 0; i < SIZE; i++)
   //   for (k = 0; k < SIZE; k++)
   //      b[i][k] = ((double)rand()/(double)RAND_MAX);
   fill_random2d_double_seed(b,42);

   print_array2d_double('b', b);

   /* initialize c by copying from b */
   //for (i = 0; i < SIZE; i++)
   //   for (k = 0; k < SIZE; k++)
   //      c[i][k] = b[i][k];

   //print_array('c', c, SIZE);

}
