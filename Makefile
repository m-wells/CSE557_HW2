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
	icc -std=c99 vi_3.c -openmp -mkl -o vi_3.out
	icc -std=c99 vi_4.c -openmp -mkl -o vi_4.out
	icc -std=c99 vi_5.c -openmp -mkl -o vi_5.out
	icc -std=c99 vi_6.c -openmp -mkl -o vi_6.out
	icc -std=c99 vi_7.c -openmp -mkl -o vi_7.out
	icc -std=c99 vi_8.c -openmp -mkl -o vi_8.out
	icc -std=c99 vi_9.c -openmp -mkl -o vi_9.out
	icc -std=c99 vi_10.c -openmp -mkl -o vi_10.out

# vim:ft=make
#

