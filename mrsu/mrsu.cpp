//28-03-2011


#include <iostream>
#include <stdio.h>
#include <math.h>
#define Pi 3.1415926535

//описание сетки
double hx = 10;
double hy = 1;
double ht = 100;

int Nx = 900;
int Ny = 200;
int N = Nx * Ny;

double lt = 0;
double t = 0;

void MSS_N(double*, double*, double*, double*, double*, double*, double*, double);
void MSS_P(double*, double*, double*, double*, double*, double*, double*, double);

int main()
{
	//погрешность
	double eps = 1e-8;
	//коэффициенты граничных условий
	double alphax = 0;
	double alphay = 0;

	double betax = 0;
	double betay = 0;
	//счетчики цикла
	int i, j;
	//расчетные массивы
	double* C = new double[N];
	double* f = new double[N];
	double* mu = new double[N] {1};
	double* u = new double[N];
	double* v = new double[N];
	double* O = new double[N];
	//коэффициенты сеточного уравнения
	double* A = new double[N];
	double* B1 = new double[N];
	double* B2 = new double[N];
	double* B3 = new double[N];
	double* B4 = new double[N];
	//правая часть
	double* F = new double[N];
	//вспомогательные переменные
	int m0, m1, m2, m3, m4, m24;
	double k1, k2, k3, k4, k0;
	//обнуляем массивы и задаем начальные условия
	for (i = 0; i < N; i++)
	{
		B1[i] = B2[i] = B3[i] = B4[i] = A[i] = F[i] = 0;
		C[i] = O[i] = f[i] = 0;
		u[i] = 0;
		v[i] = 0;
		mu[i] = 20;
	}
	//задание области
	for (i = 1; i < Nx - 2; i++)
		for (j = 1; j < Ny - 2; j++)
		{
			m0 = i + j * Nx;
			O[m0] = 1;
		}

	//задается источник
	i = Nx / 3;
	j = Ny / 3;
	m0 = i + j * Nx;
	f[m0] = 1 / (hx * hy);

	do
	{
		//построение сеточных уравнений
		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;
				m1 = m0 + 1;
				m2 = m0 - 1;
				m3 = m0 + Nx;
				m4 = m0 - Nx;
				m24 = m0 - Nx - 1;

				//mu=1, B1=(1/hx)^2, B3=B4=(1/hy)^2
				B1[m0] = 1 / (hx * hx);
				B2[m0] = B1[m0];
				B3[m0] = 1 / (hy * hy);
				B4[m0] = B3[m0];

				k0 = 0;
				A[m0] = k0 + B1[m0] + B2[m0] + B3[m0] + B4[m0];

				//F задается произвольно
				F[m0] = sin((Pi * i) / Nx) * sin((Pi * j) / Ny);

			}

		MSS_N(A, B1, B2, B3, B4, F, C, eps); ///////////////////////////////
		//MSS_P(A,B1,B2,B3,B4,F,C,eps);          ///////////////////////////////

		t = t + ht;
	} while (t <= lt - ht / 2);

	FILE* out = fopen("result.txt", "w+");
	for (i = 1; i < Nx - 1; i++)
	{
		for (j = 1; j < Ny - 1; j++)
		{
			m0 = i + j * Nx;
			fprintf(out, "%f ", C[m0]);
		}
		fprintf(out, "\n");
	}
	fclose(out);
	//расчетные массивы
	delete[] C;
	delete[] f;
	delete[] mu;
	delete[] u;
	delete[] v;
	delete[] O;
	//коэффициенты сеточного уравнения
	delete[] A;
	delete[] B1;
	delete[] B2;
	delete[] B3;
	delete[] B4;
	//правая часть
	delete[] F;

	return 0;
}

void MSS_N(double* A, double* B1, double* B2, double* B3, double* B4, double* F, double* C, double eps)
{
	int i, j, m0, m1, m2, m3, m4;
	int it = 0;
	double max;
	double Arr, Ar, rr;
	double tau;

	double* r = new double[N];
	double* w = new double[N] { 0 };
	double* alf = new double[N] { 0 };
	double* bet = new double[N] { 0 };

	for (i = 0; i < N; i++)
		r[i] = w[i] = alf[i] = bet[i] = 0;

	do
	{
		max = 0;

		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;
				m1 = m0 + 1;
				m2 = m0 - 1;
				m3 = m0 + Nx;
				m4 = m0 - Nx;

				r[m0] = F[m0] - A[m0] * C[m0] + B1[m0] * C[m1] + B2[m0] * C[m2] + B3[m0] * C[m3]
					+ B4[m0] * C[m4];

				if (max < fabs(r[m0]))
					max = fabs(r[m0]);
			}

		Arr = 0; rr = 0;

		for (i = 1; i < Nx - 1; i++)
		{
			alf[0] = 0;
			bet[0] = 0;

			for (j = 1; j < Ny - 2; j++)
			{
				m0 = i + j * Nx;
				m3 = m0 + Nx;
				m4 = m0 - Nx;

				alf[j] = B3[m0] / (A[m0] - B4[m0] * alf[j - 1]);
				bet[j] = (r[m0] + B3[m0] * bet[j - 1]) / (A[m0] - B4[m0] * alf[j - 1]);
			}
			j = Ny - 2;
			m0 = i + j * Nx;

			w[m0] = (r[m0] + B3[m0] * bet[j - 1]) / (A[m0] - B4[m0] * alf[j - 1]);

			for (j = Ny - 3; j > 0; j--)
			{
				m0 = i + j * Nx;
				m3 = m0 + Nx;
				w[m0] = alf[j] * w[m3] + bet[j];
			}
		}

		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;
				m1 = m0 + 1;
				m2 = m0 - 1;
				m3 = m0 + Nx;
				m4 = m0 - Nx;

				Ar = A[m0] * w[m0] - (B1[m0] * w[m1] + B2[m0] * w[m2] + B3[m0] * w[m3] + B4[m0] * w[m4]);

				Arr += Ar * w[m0];
				rr += w[m0] * r[m0];
			}

		if (Arr != 0)
			tau = rr / Arr;
		else
			printf("\n\nZERO\n\n");

		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;

				C[m0] = C[m0] + tau * w[m0];
			}
		it++;

	} while (max > eps);

	printf("t=%f it=%d\n", t, it);


	delete[]r;
}

void MSS_P(double* A, double* B1, double* B2, double* B3, double* B4, double* F, double* C, double eps)
{
	int i, j, m0, m1, m2, m3, m4;
	int it = 0;
	double max;
	double Arr, Ar, rr;
	double tau;

	double* r = new double[N];

	for (i = 0; i < N; i++)
		r[i] = 0;

	do
	{
		max = 0;

		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;
				m1 = m0 + 1;
				m2 = m0 - 1;
				m3 = m0 + Nx;
				m4 = m0 - Nx;

				r[m0] = F[m0] - A[m0] * C[m0] + B1[m0] * C[m1] + B2[m0] * C[m2] + B3[m0] * C[m3]
					+ B4[m0] * C[m4];

				if (max < fabs(r[m0]))
					max = fabs(r[m0]);

				if (A[m0] > 0)
					r[m0] = r[m0] / A[m0]; //предобуславливатель
			}

		Arr = 0; rr = 0;

		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;
				m1 = m0 + 1;
				m2 = m0 - 1;
				m3 = m0 + Nx;
				m4 = m0 - Nx;

				Ar = A[m0] * r[m0] - (B1[m0] * r[m1] + B2[m0] * r[m2] + B3[m0] * r[m3]
					+ B4[m0] * r[m4]);

				Arr += Ar * r[m0];
				rr += A[m0] * r[m0] * r[m0];
			}

		if (Arr > 0) tau = rr / Arr;

		for (i = 1; i < Nx - 1; i++)
			for (j = 1; j < Ny - 1; j++)
			{
				m0 = i + j * Nx;

				C[m0] = C[m0] + tau * r[m0];
			}
		it++;

	} while (max > eps);

	printf("t=%f it=%d\n", t, it);

	delete[]r;
}
