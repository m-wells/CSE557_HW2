#
# Makefile
# gks, 2014-04-27 21:33
#

all:
	@echo "Compiling v.c with icc"
	icc -std=c99 -o v.out v.c -L/usr/global/intel/mkl/10.2.6.038/lib/em64t -Wl,-R/usr/global/intel/mkl/10.2.6.038/lib/em64t -lmkl_lapack -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lmkl_sequential -lm


# vim:ft=make
#

