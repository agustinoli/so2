SHELL := /bin/bash

compile:
	gcc -o readNetcdf.o readNetcdf.c -O3 -lm -lnetcdf -fopenmp -Wall -Werror -pedantic
test:
	rm file.nc -f
	cp OR_ABI-L2-CMIPF-M6C02_G16_s20191011800206_e20191011809514_c20191011809591.nc file.nc
	time ./readNetcdf.o 1
clean:
	rm *.o -f
	rm run_*.txt -f
check:
	cppcheck readNetcdf.c --enable=all
show:
	python3 show.py
profile:
	gcc readNetcdf.c -o profiling.o -g -pg -std=c99 -O3 -lm -lnetcdf -fopenmp -Wall -Werror -pedantic
