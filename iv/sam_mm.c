/**
 * File Name: sam_mm.c
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
 * cc sam_mm.c -o sam_mm -lmpi
 *
 * Comments: I've put lots of effort to make the code as easy to read as 
 * possible. So many opportunities for optimiaztions are possible, e.g.
 * all the dynmic checking of ownership can be eliminated, and even the
 * memcpy(...) can be removed.
 *
 * -Hong Tang, May. 1999
 */

# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>

static int n;	 	 /* size of the matrix */
static int bsize;	 /* size of the sub-matrix */
static int bnum;	 /* number of sub-matrix columns/rows in the matrix */ 
static int nproc;	 /* number of MPI nodes */
static int myid; 	 /* my own rank */

static double *A, *B, *C, *buf; /* buf is the temporary storage */

/* need only one local mapping because A/B/C all use block mapping */
int LOCAL(int i)
{
	return i%(n/nproc);
}

/* Block version */
int LOCALB(int i)
{
	return i%(bnum/nproc);
}

/* get the starting address of submatrix A[i][j], i, j are local indices */
double *MAP_BLK_A(int i, int j)
{	
	return A+(i*bnum+j)*bsize*bsize;
}

/* get the starting address of submatrix B[i][j], i, j are local indices */
double *MAP_BLK_B(int i, int j)
{	
	return B+(j*bnum+i)*bsize*bsize;
}

/* get the starting address of submatrix C[i][j], i, j are local indices */
double *MAP_BLK_C(int i, int j)
{	
	return C+(i*bnum+j)*bsize*bsize;
}

/* get the starting address of submatrix buf[k] */
double *MAP_BLK_BUF(int k)
{	
	return buf+k*bsize*bsize;
}

/* who is the guy owns A[i][j]? */
int OWNER_A(int i, int j)
{
	return i/(n/nproc);
}

/* who is the guy owns B[i][j]? */
int OWNER_B(int i, int j)
{
	return j/(n/nproc);
}

/* who is the guy owns C[i][j]? */
int OWNER_C(int i, int j)
{
	return i/(n/nproc);
}

/* get the address of element A[k][l] */
double * MAP_ELEM_A(int k, int l)
{
	int local_i=LOCAL(k), local_j=l;
	int local_bi=local_i/bsize, local_bj=local_j/bsize;
	int blk_i=local_i%bsize, blk_j=local_j%bsize;
	
	return MAP_BLK_A(local_bi, local_bj)+blk_i*bsize+blk_j;
}

/* get the address of element B[k][l] */
double * MAP_ELEM_B(int k, int l)
{
	int local_i=k, local_j=LOCAL(l);
	int local_bi=local_i/bsize, local_bj=local_j/bsize;
	int blk_i=local_i%bsize, blk_j=local_j%bsize;
	
	return MAP_BLK_B(local_bi, local_bj)+blk_i*bsize+blk_j;
}

/* get the address of element C[k][l] */
double * MAP_ELEM_C(int k, int l)
{
	int local_i=LOCAL(k), local_j=l;
	int local_bi=local_i/bsize, local_bj=local_j/bsize;
	int blk_i=local_i%bsize, blk_j=local_j%bsize;
	
	return MAP_BLK_C(local_bi, local_bj)+blk_i*bsize+blk_j;
}

void do_sub_mm(double *c, double *a, double *b)
{
	int i, j, k;

	/* lots of constants can be moved out from the inner most loop */
	/* for easy-understanding, I didn't do that */
	for (i=0; i<bsize; i++)
		for (j=0; j<bsize; j++)
			for (k=0; k<bsize; k++)
				c[i*bsize+j]+=a[i*bsize+k]*b[k*bsize+j];
}

void printC()
{
	int i, j;

	for (i=0; i<n; i++)
		if (OWNER_C(i, 0)==myid) {
			for (j=0; j<n; j++)
				printf("%4f  ", *MAP_ELEM_C(i,j));
			printf("\n");
		}
}

int main(int argc, char *argv[])
{
	int i, j, k, token;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	if (argc!=3) {
		if (myid==0)
			fprintf(stderr, "Usage, %s <n> <n/q>.\n", argv[0]);
		MPI_Finalize();
		return -1;
	}

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

	/* initialize A and B, this is inefficient but easy to understand */
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) {
			if (OWNER_A(i,j)==myid) {
				*MAP_ELEM_A(i,j)=j;
			}
			if (OWNER_B(i,j)==myid) {
				*MAP_ELEM_B(i,j)=i+j;
			}
	}

	if (myid==0) {
		printf("Done!\n\n");
		printf("Multiplication ...\n");
	}

	for (j=0; j<bnum; j++)
	{
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

	MPI_Finalize();

	return 0;
}
