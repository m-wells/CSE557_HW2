#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
/* macro for column major indexing */
#define A( i,j ) (a[ j*lda + i ])
#define B( i,j ) (b[ j*ldb + i ])
#define C( i,j ) (c[ j*ldc + i ])
#define min( x, y ) ( (x) < (y) ? (x) : (y) )
extern void RING_Bcast( double *buf, int count, MPI_Datatype type, int root, MPI_Comm comm );
extern void dlacpy_( char* uplo, int* m, int* n, double* a, int* lda, double* b, int* ldb );
extern void dgemm_( char* transa, char* transb, int* m, int* n, int* k, double* alpha, double* a, int* lda, double* b, int* ldb, double* beta, double* c, int* ldc );

/* BEFORE ASYNC
 *             m = 100 n = 100 k = 100 process grid (2,2)
 *             with blocks: nb_m = 50, nb_n = 50, nb_k = 50, nb_alg = 50
 *             time (s) MPI SCALAPACK               = 0.011912 
 *             time (s) MPI SUMMA                   = 0.004739 
 *             error (I,1,F) SCALAPACK/SUMMA        = 7.854316e-02 8.629512e-02 8.338418e-02 
 */

/**/
void summa_pdgemm( m, n, k, nb, alpha, a, lda, b, ldb,
		beta, c, ldc, m_a, n_a, m_b, n_b, m_c, n_c,
		comm_row, comm_col, work1, work2, nprow, npcol )
/**/
int    m, n, k,               /* global matrix dimensions */
       nb,                    /* panel width */
       m_a[], n_a[],          /* dimensions of blocks of A */
       m_b[], n_b[],          /* dimensions of blocks of A */
       m_c[], n_c[],          /* dimensions of blocks of A */
       lda, ldb, ldc,         /* leading dimension of local arrays that hold local portions of matrices A, B, C */
       nprow, npcol;
double *a, *b, *c,            /* arrays that hold local parts of A, B, C */
       alpha, beta,           /* multiplication constants */
       *work1, *work2;        /* work arrays */
       MPI_Comm comm_row,     /* Communicator for this row of nodes */
       comm_col;              /* Communicator for this column of nodes */
{
double d_one=1.0,
       d_zero=0.0; /* used for constant passed to blas call */
	int myrow, mycol,     /* my row and column index */
	    i, j, kk, iwrk,   /* misc. index variables */
	    icurrow, icurcol, /* index of row and column that hold current row and column, resp., for rank-1 update*/
	    ii, jj;           /* local index (on icurrow and icurcol, resp.) of row and column for rank-1 update */

	/* double *temp; */         /* temporary pointer used in pdgemm_abt */
	/* get myrow, mycol */
	MPI_Comm_rank( comm_row, &mycol ); MPI_Comm_rank( comm_col, &myrow );
	/* scale local block of C */
	for ( j=0; j<n_c[ mycol ]; j++ )
		for ( i=0; i<m_c[ myrow ]; i++ )
			C( i,j ) = beta * C( i,j );
	icurrow = 0; icurcol = 0;
	ii = jj = 0;
	/* malloc temp space for summation */
	/* temp = (double *) malloc( m_c[myrow]*nb*sizeof(double) ); */
	for ( kk=0; kk<k; kk+=iwrk) {
		/* iwrk = min( nb, m_b[ icurrow ]-ii );
		 * iwrk = min( iwrk, n_a[ icurcol ]-jj );
		 */
		iwrk = min ( nb , k-kk ) ;
		/* pack current iwrk columns of A into work1 */
		if ( mycol == icurcol )
			dlacpy_( "General", &m_a[ myrow ], &iwrk, &A( 0, jj ), &lda, work1, &m_a[ myrow ] );
		/* pack current iwrk rows of B into work2 */
		if ( myrow == icurrow )
			dlacpy_( "General", &iwrk, &n_b[ mycol ], &B( ii, 0 ), &ldb, work2, &iwrk );
		/* broadcast work1 and work2 */
		{
			/* Position 0 is for the communications on comm_row and position 1 is for the comm_col */
			MPI_Request req[2] = {MPI_REQUEST_NULL, MPI_REQUEST_NULL };
			MPI_Status status[2];
			int loop, index, count = 0;
			int row_send_done = 0, col_send_done = 0;

			if ( mycol != icurcol ) {
				MPI_Irecv( work1, m_a[ mycol ]*iwrk, MPI_DOUBLE, (mycol-1+nprow)%nprow, MPI_ANY_TAG, comm_row, &(req[0]) );
				count++;
			} else {
				if ( ( mycol+1 )%nprow != icurcol ) {
					MPI_Isend( work1, m_a[ mycol ]*iwrk, MPI_DOUBLE, (mycol+1)%nprow, 0, comm_row, &(req[0]) );
					count++;
					row_send_done = 1;
				}
			}

			if ( myrow != icurrow ) {
				MPI_Irecv( work2, n_b[ myrow ]*iwrk, MPI_DOUBLE, (myrow-1+npcol)%npcol, MPI_ANY_TAG, comm_col, &(req[1]) );
				count++;
			} else {
				if ( ( myrow+1 )%npcol != icurrow ) {
					MPI_Isend( work2, n_b[ myrow ]*iwrk, MPI_DOUBLE, (myrow+1)%npcol, 0, comm_col, &(req[1]) );
					count++;
					col_send_done = 1;
				}
			}
			for( loop = 0; loop < count; loop++ ) {
				MPI_Waitany( 2, req, &index, status );
				if( 0 == index ) {
					if ( (row_send_done == 0) && (( mycol+1 )%nprow != icurcol) ) {
						MPI_Isend( work1, m_a[ mycol ]*iwrk, MPI_DOUBLE, (mycol+1)%nprow, 0, comm_row, &(req[0]) );
						fprintf( stdout, "generate isend for request[0]\n" ); fflush( stdout );
						if( MPI_REQUEST_NULL != req[0] ) count++;
						row_send_done = 1;
					}
				} else if( 1 == index ) {
					if ( (col_send_done == 0) && (( myrow+1 )%npcol != icurrow) ) {
						fprintf( stdout, "generate isend for request[1]\n" ); fflush( stdout );
						MPI_Isend( work2, n_b[ myrow ]*iwrk, MPI_DOUBLE, (myrow+1)%npcol, 0, comm_col, &(req[1]) );
						if( MPI_REQUEST_NULL != req[1] ) count++;
						col_send_done = 1;
					}
				} else {
					fprintf( stdout, "It should never happens (index %d:count %d)!!!\n", index, count ); fflush( stdout );
				}
			}
		}
#if 0
		RING_Bcast( work1 , m_a[ myrow ]*iwrk, MPI_DOUBLE, icurcol, comm_row );
		RING_Bcast( work2 , n_b[ mycol ]*iwrk, MPI_DOUBLE, icurrow, comm_col );
#endif
		/* update local block */
		if (m_c[ myrow ] >0 ){
			dgemm_( "No transpose", "No transpose", &m_c[ myrow ], &n_c[ mycol ],
				&iwrk, &alpha, work1, &m_a[ myrow ], work2, &iwrk, &d_one,
				c, &ldc );
		}
		/* update icurcol, icurrow, ii, jj */
		/*
		 * ii += iwrk; jj += iwrk;
		 * if ( jj>=n_a[ icurcol ] ) { icurcol++; jj = 0; };
		 * if ( ii>=m_b[ icurrow ] ) { icurrow++; ii = 0; };
		 */
		if ( myrow == icurrow ) ii += iwrk ;
		if ( mycol == icurcol ) jj += iwrk ;
		icurrow = ( icurrow+1 )%nprow ; /* nope! Just if we have effectively finished the small block */
		icurcol = ( icurcol+1 )%npcol ; /* nope! Just if we have effectively finished the small block */
	}
	/* free( temp ); */
}
/**/
void RING_Bcast( double *buf, int count, MPI_Datatype type, int root, MPI_Comm comm )
{
	int me, np;
	MPI_Status status;
	MPI_Comm_rank( comm, &me ); MPI_Comm_size( comm, &np );
	if ( me != root)
		MPI_Recv( buf, count, type, (me-1+np)%np, MPI_ANY_TAG, comm, &status );
	if ( ( me+1 )%np != root )
		MPI_Send( buf, count, type, (me+1)%np, 0, comm );
}
