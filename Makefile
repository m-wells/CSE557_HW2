#
# Makefile
# gks, 2014-04-27 21:33
#
#
#	icc -std=c99 v.c -openmp -L/usr/global/intel/mkl/10.3.12.361/lib/intel64 -Wl,-R/usr/global/intel/mkl/10.3.12.361/lib/intel64 -lmkl_solver_lp64_sequential -Wl,--start-group -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -Wl,--end-group -o vvv.out
#
#export MKLPATH:=$MKLROOT/lib
#export MKLINCLUDE=$MKLROOT/include
#icc -std=c99 v.c -openmp -lmkl_solver_lp64_sequential -Wl,--start-group -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -Wl,--end-group -o vvv.out

all:
	@echo "Compiling v.c with icc"
	icc -std=c99 vi.c -openmp -mkl -o vi.out
	icc -std=c99 vi_1.c -openmp -mkl -o vi_1.out
	icc -std=c99 vi_2.c -openmp -mkl -o vi_2.out

# vim:ft=make
#

