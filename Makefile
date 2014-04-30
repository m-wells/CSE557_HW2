# Makefile
# Initially created by Gudmundur, 2014-04-27 21:33
# 
# A few notes for running on the RCC:
# iv:
#  - You will need to do 
#  > module load mpich2
#
# vi:
#  - You will need to set the Intel MKL environment variables:
#

all:  
	@echo "Compiling iii.c with icc"
	icc -std=c99 iii/iii.c  -openmp -o   iii/iii.out
	icc -std=c99 iii/iii_1.c  -openmp -o iii/iii_1.out
	icc -std=c99 iii/iii_2.c  -openmp -o iii/iii_2.out
	icc -std=c99 iii/iii_3.c  -openmp -o iii/iii_3.out
	icc -std=c99 iii/iii_4.c  -openmp -o iii/iii_4.out
	icc -std=c99 iii/iii_5.c  -openmp -o iii/iii_5.out
	icc -std=c99 iii/iii_6.c  -openmp -o iii/iii_6.out
	icc -std=c99 iii/iii_7.c  -openmp -o iii/iii_7.out
	icc -std=c99 iii/iii_8.c  -openmp -o iii/iii_8.out
	icc -std=c99 iii/iii_9.c  -openmp -o iii/iii_9.out
	icc -std=c99 iii/iii_10.c -openmp -o iii/iii_10.out
	@echo "Compiling iv.c with mpicc"
	

#vi: vi_1.out 
#	@echo "Compiling vi.c with icc"
#	icc -std=c99 vi/vi.c -mkl -o vi/vi.out
#	icc -std=c99 vi/vi_1.c  -openmp -mkl -o vi/vi_1.out
#	icc -std=c99 vi/vi_2.c  -openmp -mkl -o vi/vi_2.out
#	icc -std=c99 vi/vi_3.c  -openmp -mkl -o vi/vi_3.out
#	icc -std=c99 vi/vi_4.c  -openmp -mkl -o vi/vi_4.out
#	icc -std=c99 vi/vi_5.c  -openmp -mkl -o vi/vi_5.out
#	icc -std=c99 vi/vi_6.c  -openmp -mkl -o vi/vi_6.out
#	icc -std=c99 vi/vi_7.c  -openmp -mkl -o vi/vi_7.out
#	icc -std=c99 vi/vi_8.c  -openmp -mkl -o vi/vi_8.out
#	icc -std=c99 vi/vi_9.c  -openmp -mkl -o vi/vi_9.out
#	icc -std=c99 vi/vi_10.c -openmp -mkl -o vi/vi_10.out
#
#vii:
#	@echo "Compiling vii.c with icc"
#	mpicc        -L$MKLROOT/lib/intel64 -lmkl_scalapack_lp64 \
#	  -lmkl_blacs_intelmpi_lp64 \
#	  -lmkl_intel_lp64 \
#	  -lmkl_intel_thread -lmkl_core \
#	  -liomp5 -lpthread

clean:
	rm -rf i/*.out
	rm -rf ii/*.out
	rm -rf iii/*.out
	rm -rf vi/*.out
	rm -rf vii/*.out
	
# vim:ft=make
#
