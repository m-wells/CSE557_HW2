/* print_array: print a 2-dimensional array */
void print_array2d_double(char ch, double a[SIZE][SIZE]) {
   int i, j;
   /* print the array header */
   printf("Array %c:  ", ch);
   for (i = 0; i < SIZE; i++) printf(" Col %i ", i);
   printf("\n\n");
   /* print each row of the array */
   for (i = 0; i < SIZE; i++) {
      printf("Row %i:    ", i);
      for (j = 0; j < SIZE; j++)
         printf("%6f ", a[i][j]);
          /* must add newline at end */
      printf("\n");
   }
   printf("\n");
}

void fill_random2d_double(double a[SIZE][SIZE]) {
   int i, k;
   srand((unsigned)time(NULL));
   for (i = 0; i < SIZE; i++){
      for (k = 0; k < SIZE; k++) {
         a[i][k] = ((double)rand()/(double)RAND_MAX); 
      }
   }
}

void fill_random2d_double_seed(double a[SIZE][SIZE],int myseed) {
   int i, k;
   srand(myseed);
   for (i = 0; i < SIZE; i++){
      for (k = 0; k < SIZE; k++) {
         a[i][k] = ((double)rand()/(double)RAND_MAX); 
      }
   }
}
