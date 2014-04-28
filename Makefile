#
# Makefile
# gks, 2014-04-27 21:33
#

all:
	@echo "Compiling v.c with icc"
	icc -std=c99 -o vvv v.c -openmp -L/usr/global/intel/mkl/10.3.12.361/lib/intel64 -Wl,-R/usr/global/intel/mkl/10.3.12.361/lib/intel64 -lmkl_solver_lp64_sequential -Wl,--start-group -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -Wl,--end-group vvv.out


# vim:ft=make
#

