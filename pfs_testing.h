#include <time.h>
#include <stdlib.h>

void random_fill_array(int *list, int len)
{
	int i;
	time_t t;
	srand((unsigned) time(&t));
	for (i = 0; i < len; i++) {
		list[i] = rand() % 100;
	}
}

//void random_fill_array_double(double *list, int len)
//{
//	int i;
//	srand48(time(NULL));
//	for (i = 0; i < len; i++) {
//		list[i] = drand48();
//	}
//}

void print_clock(double launch, double done)
{
    double diff = (done - launch) / CLOCKS_PER_SEC;
    printf("time elasped: %f\n",diff);
}

void print_list_int(int list[], int len)
{
    for (int i = 0; i < len ; i++) {
        printf("%6d",list[i]);
    }
    printf("\n"); 
}

void print_list_double(double list[], int len)
{
    for (int i = 0; i < len ; i++) {
        printf("%9f ",list[i]);
    }
    printf("\n"); 
}

double mean_double (double *list, int n)
{
	double sum = 0.0;
	double mean;
	for (int i = 0; i < n; i++) {
		sum = sum + list[i];
	}
        mean = sum/n;
	return mean;
}

double mean_int (int *list, int n)
{
	double sum = 0.0;
	double mean;
	for (int i = 0; i < n; i++) {
		sum = sum + list[i];
	}
        mean = sum/n;
	return mean;
}

int match_test(int *list_serial, int *list_parallel, int len)
//Purpose:  Tests to make sure the arrays are identical. 
//Input:
//  *list_serial    - array of serial results
//  *list_parallel  - array of parallel results
//Output:
//  Returns 1 if different, returns 0 if equal.
{
	int diff;
	for (int i = 0; i < len; i++) {
		diff = (list_serial[i] - list_parallel[i]);
        if (diff != 0) {
            return 1;
        }
	}
	return 0;
}

void array_copy_int(int *src, int *dst, int len){
//Purpose: Copies one array into another.
//Input:
//  *src    - the source array
//  *dst    - the destination array
//  len     - the length of both arrays.

    //make a copy of src and put it into dst
    //works for int arrays only
    for (int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}
