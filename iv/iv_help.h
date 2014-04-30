/* need only one local mapping because A/B/C all use block mapping */
int LOCAL(int i) {
	return i%(n/nproc);
}

/* Block version */
int LOCALB(int i) {
	return i%(bnum/nproc);
}

/* get the starting address of submatrix A[i][j], i, j are local indices */
double *MAP_BLK_A(int i, int j) {	
	return A+(i*bnum+j)*bsize*bsize;
}

/* get the starting address of submatrix B[i][j], i, j are local indices */
double *MAP_BLK_B(int i, int j) {	
	return B+(j*bnum+i)*bsize*bsize;
}

/* get the starting address of submatrix C[i][j], i, j are local indices */
double *MAP_BLK_C(int i, int j) {	
	return C+(i*bnum+j)*bsize*bsize;
}

/* get the starting address of submatrix buf[k] */
double *MAP_BLK_BUF(int k) {	
	return buf+k*bsize*bsize;
}

/* who is the guy owns A[i][j]? */
int OWNER_A(int i, int j) {
	return i/(n/nproc);
}

/* who is the guy owns B[i][j]? */
int OWNER_B(int i, int j) {
	return j/(n/nproc);
}

/* who is the guy owns C[i][j]? */
int OWNER_C(int i, int j) {
	//printf("C owns %d\n", i/(n/nproc));
	return i/(n/nproc);
}

/* get the address of element A[k][l] */
double * MAP_ELEM_A(int k, int l) {
	int local_i=LOCAL(k), local_j=l;
	int local_bi=local_i/bsize, local_bj=local_j/bsize;
	int blk_i=local_i%bsize, blk_j=local_j%bsize;
	return MAP_BLK_A(local_bi, local_bj)+blk_i*bsize+blk_j;
}

/* get the address of element B[k][l] */
double * MAP_ELEM_B(int k, int l) {
	int local_i=k, local_j=LOCAL(l);
	int local_bi=local_i/bsize, local_bj=local_j/bsize;
	int blk_i=local_i%bsize, blk_j=local_j%bsize;
	return MAP_BLK_B(local_bi, local_bj)+blk_i*bsize+blk_j;
}

/* get the address of element C[k][l] */
double * MAP_ELEM_C(int k, int l) {
	int local_i=LOCAL(k), local_j=l;
	int local_bi=local_i/bsize, local_bj=local_j/bsize;
	int blk_i=local_i%bsize, blk_j=local_j%bsize;
	
	return MAP_BLK_C(local_bi, local_bj)+blk_i*bsize+blk_j;
}

/* Calculate the submatrix */
void do_sub_mm(double *c, double *a, double *b) {
	int i, j, k;
	/* lots of constants can be moved out from the inner most loop */
	/* for easy-understanding, I didn't do that */
	for (i=0; i<bsize; i++)
		for (j=0; j<bsize; j++)
			for (k=0; k<bsize; k++)
				c[i*bsize+j]+=a[i*bsize+k]*b[k*bsize+j];
}
void printA() {
	int i, j;
	for (i=0; i<n; i++)
		if (OWNER_A(i, 0)==myid) {
			for (j=0; j<n; j++)
				printf("%4f  ", *MAP_ELEM_A(i,j));
			printf("\n");
		}
}
void printB() {
	int i, j;
	for (i=0; i<n; i++)
		if (OWNER_B(i, 0)==myid) {
			for (j=0; j<n; j++)
				printf("%4f  ", *MAP_ELEM_B(i,j));
			printf("\n");
		}
}
void printC() {
	int i, j;
	for (i=0; i<n; i++)
		if (OWNER_C(i, 0)==myid) {
			for (j=0; j<n; j++)
				printf("%4f  ", *MAP_ELEM_C(i,j));
			printf("\n");
		}
}
