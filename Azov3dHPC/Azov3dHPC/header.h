#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include <stdio.h>
#include <conio.h>


void vvod(double*, double*, double*,
	double*, double*, double*,
	double*, double*, double*,
	double*, int*);

void vvodmask(double*, double*, double*,
	double*, double*, double*, int*);

void vivod(double*, double*, double*, double*);


void firstfunc(double*, double*,
	double*, double*, double*, double*, int*);

void secondfunc(double*, double*, double*, double*, double*, double*, int*);

void thirdfunc(double*, double*, double*, double*,
	double*, int*);


void ptmgs(double*, double*, double*, double*, double*,
	double*, double*, double*, double*, double*,
	double*, int*, double, double);

void ptmss(double*, double*, double*, double*, double*,
	double*, double*, double*,
	double*, int*, double, double);

void ptmsp(double*, double*, double*, double*, double*,
	double*, double*, double*, double*, double*,
	double*, int*, double, double);

void secondfunc2d(double*, double*, double*, double*, double*, double*, int*);

void ptmgs2d(double*, double*, double*, double*, double*,
	double*, double*, int*, double, double);


//////////////////////////////////////////////////////////////////////////////////////
cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size);