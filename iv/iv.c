/**
 * File Name: sam_mm.c //NOW: iv.c
 * Sample matrix multiplication code.
 * Algorithm: A and C are partitioned in 1D row blocks, B is partitioned in 
 * 1D column blocks. 
 *
 * The main multiplication involves bnum iterations, 
 * in iteration i, the ith sub-matrix column of B is broadcast to everybody 
 * then everybody uses this column and its own portion of A to update the ith 
 * column of its own portion of C.
 *
 * To compile:
 * cc iv.c -o sam_mm -lmpi
 *
 * Comments: I've put lots of effort to make the code as easy to read as 
 * possible. So many opportunities for optimiaztions are possible, e.g.
 * all the dynmic checking of ownership can be eliminated, and even the
 * memcpy(...) can be removed.
 *
 * -Hong Tang, May. 1999
 *
 * -------------------------------------------------------------------------
 * -------------------------------------------------------------------------
 * -------------------------------------------------------------------------
 * //EDITS FOR CSE557 April 2014:
 * - Put helper functions into "iv_help.h" to keep it more clean
 * - You have to include the two #include statements as well (not included
 *   in Tang's code 1999.
 *
 * - We compiled it in the following way on the RCC:
 *   > module load mpich2
 *   > mpicc vi.c -o iv.out
 *
 * - To run with a matrix of size 8000, and 1D block-size of 25 on P threads, 
 *   > mpirun -n P ./iv.out 8000 25
 * -------------------------------------------------------------------------
 * -------------------------------------------------------------------------
 * -------------------------------------------------------------------------
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Have to include this additionally from Tang's code 1999
#include <unistd.h> // Have to include this additionally from Tang's code 1999

static int n;	 	 /* size of the matrix */
static int bsize;	 /* size of the sub-matrix */
static int bnum;	 /* number of sub-matrix columns/rows in the matrix */ 
static int nproc;	 /* number of MPI nodes */
static int myid; 	 /* my own rank */

static double *A, *B, *C, *buf; /* buf is the temporary storage */

#include "iv_help.h"

int main(int argc, char *argv[])
{
	int i, j, k, token;
	MPI_Status status;
	double start_time = 0;
	double end_time = 0;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	if (argc!=3) {
		if (myid==0)
			fprintf(stderr, "Usage, %s <n> <n/q>.\n", argv[0]);
		MPI_Finalize();
		return -1;
	}

        printf("Hello world from rank %3d of %3d\n", myid, nproc);
	n=atoi(argv[1]);
	bsize=atoi(argv[2]);

	bnum=n/bsize; /* For simplicity, assume they are all correct. */
	if ( (bnum*bsize!=n) || (bnum/nproc*nproc!=bnum) ) {
		if (myid==0)
			fprintf(stderr, "Invalid parameters <nproc=%d> <n=%d> <n/q=%d>.\n", nproc, n, bsize);
		MPI_Finalize();
		return -1;
	}
	/* assuming bnum divisable by nproc */
	
	A=(double *)malloc(sizeof(double)*(n*n/nproc));
	B=(double *)malloc(sizeof(double)*(n*n/nproc));
	C=(double *)malloc(sizeof(double)*(n*n/nproc));
	buf=(double *)malloc(sizeof(double)*(bsize*n));

	if (!(A && B && C && buf)) {
		if (myid==0)
			fprintf(stderr, "Out of Memory.\n");
		MPI_Finalize();
		return -1;
	}
	/* zero the memory used by C */
	memset((char *)C, 0, sizeof(double)*(n*n/nproc));

	if (myid==0) {
		printf("Initialization ...\n");
	}
	//-------------------------------------------------------------------
	/* initialize A and B */
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) {
			if (OWNER_A(i,j)==myid) {
				//printf("A owns %d\n", i/(n/nproc));
				*MAP_ELEM_A(i,j)=1;
			}
			if (OWNER_B(i,j)==myid) {
				//printf("B owns %d\n", j/(n/nproc));
				*MAP_ELEM_B(i,j)=2;
			}
	}
	// Printing
	if (myid==0) {
		printf("Done!\n\n");
		printf("Multiplication ...\n");
        	start_time = MPI_Wtime();
	}

	// Multiplication
	for (j=0; j<bnum; j++) {
		int root=OWNER_B(0, j*bsize);
		if (root==myid)
			memcpy(buf, MAP_BLK_B(0, LOCALB(j)), sizeof(double)*(bsize*n)) ;

		MPI_Bcast(buf, bsize*n, MPI_DOUBLE, root, MPI_COMM_WORLD);

		for (i=0; i<bnum/nproc; i++) {
			/* update C[i][j] */
			for (k=0; k<bnum; k++) 
				/* C[i][j]+=A[i][k]*buf[k] */
				do_sub_mm(MAP_BLK_C(i, j), MAP_BLK_A(i, k), MAP_BLK_BUF(k));
		}
	}

	//-------------------------------------------------------------------
	if (myid==0) {
		printf("Done!\n\n");
	}
	if (n<=10) {
		if (myid==0)
			printf("Printing Matrix C ...\n");
		/* implementing a token pass protocol to print the C */
		if (myid!=0) {
			MPI_Recv(&token, 1, MPI_INT, myid-1, 2, MPI_COMM_WORLD, &status);
		}
		printC();
		fflush(stdout);
	
		if (myid<nproc-1) {
			MPI_Send(&token, 1, MPI_INT, myid+1, 2, MPI_COMM_WORLD);
		}
	
		if (myid==nproc-1) {
			printf("Done!\n");
		}
	}
	if (myid==0) {
		printf("Done!\n\n");
		end_time = MPI_Wtime() - start_time;
		printf("Time, %f \n", end_time);
	}

	MPI_Finalize();

	return 0;
}
