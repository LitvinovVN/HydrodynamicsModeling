#include "header.h"
#include "source.cpp"

#pragma region Глобальные переменные

int n1 = 355;// количество точек по оси X
int n2 = 233;// количество точек по оси Y
int n3 = 36;// количество точек по оси Z

int n = n1 * n2 * n3;// количество точек
int n2d = n1 * n2;// количество точек в плоскости XY

double      dt = 500;// шаг по времени ???????
double      t = 0;// начало моделирования по времени
double      dx = 1000;//6800/n1;// шаг по X ????
double      dy = 1000;//280/n2; // шаг по Y ????
double      dz = 0.5; //280/n2; // шаг по Z ????
double      t0 = 1000;//1000000; // расчетный отрезок времени
double      pa = 1.29;// плотность атмосферы
double      po = 1000;// плотность водной среды
double      pv = 2700;// плотность взвеси
double      ev = 0.00000000001;// ???
double      ep = 0.00000000001;// ??
double      ek = 0.000001;// ??
double      kor = 0.000103;// коэффициент силы Кориолиса
double      cp_v = 0.0088;//????
double      cp_a = 0.0026;//????
double      g = 9.81;// ускорение сободного падения

double      sk_u = 0;//-0.1; компонента скорости ветра (проекция скорости ветра на ось X)
double      sk_v = -40;// компонента скорости ветра (проекция скорости ветра на ось Y)

double      ca = pa * cp_a / po;// коэффициент силы трения о воздух
double      cv = pv * cp_v / po;// коэффициент силы трения о дно
double      isp = 606 / (38869 * dx * dy);// коэффициент испарения
#pragma endregion

#pragma region CUDA-ядра
__global__ void addKernel(int* c, const int* a, const int* b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}
#pragma endregion

int main()
{
    double* u = new double[n];// массив проекций вектора скорости водного потока на ось X
    double* v = new double[n];// массив проекций вектора скорости водного потока на ось Y
    double* w = new double[n];// массив проекций вектора скорости водного потока на ось Z
    double* a = new double[n];// массив значений давления (поле давления)
    double* a1 = new double[n2d];// массив значений гидростатического давления (поле гидростатического давления)
    double* mu = new double[n];// массив значений вертикальной составляющей поля коэффициента турбулентного обмена
    double* nu = new double[n];// массив значений горизонтальной составляющей поля коэффициента турбулентного обмена
    double* o_max = new double[n];//????????????????????????????????????????????????
    double* o = new double[n];// ????
    double* h = new double[n2d];// массив значений глубины

    int* s1 = new int[n];// массив маркеров типа ячейки (0 - ячейка не входит в расчетную область, 1 - ячейка входит в расчетную область, 2 - ??????)

    int k1;

    printf("mass OK\n");

    vvod(u, v, w, a, a1, mu, nu, o, o_max, h, s1);
    printf("vvod OK\n");

	do
	{
		vvodmask(u, v, w, a, o, o_max, s1);

		firstfunc(u, v, w, mu, nu, o, s1);
		secondfunc2d(u, v, w, a, a1, o, s1);
		secondfunc(u, v, w, a, a1, o, s1);
		thirdfunc(u, v, w, a, o, s1);

		t = t + dt;

		vivod(u, v, w, a);
		printf("vivod OK\n");

	} while (t < t0);


	delete[] u;
	delete[] v;
	delete[] w;
	delete[] a;
	delete[] a1;
	delete[] s1;
	delete[] mu;
	delete[] nu;
	delete[]o_max;
	delete[]o;
	delete[]h;

	printf("del OK\n");

















    /////////////////////////////////////////////
    const int arraySize = 5;
    const int aa[arraySize] = { 1, 2, 3, 4, 5 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, aa, b, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size)
{
    int* dev_a = 0;
    int* dev_b = 0;
    int* dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel << <1, size >> > (dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);

    return cudaStatus;
}



/// <summary>
/// Ввод начальных условий
/// </summary>
/// <param name="u"></param>
/// <param name="v"></param>
/// <param name="w"></param>
/// <param name="a"></param>
/// <param name="a1"></param>
/// <param name="mu"></param>
/// <param name="nu"></param>
/// <param name="o"></param>
/// <param name="o_max"></param>
/// <param name="h"></param>
/// <param name="s"></param>

void vvod(double* u,
	double* v,
	double* w,
	double* a,
	double* a1,
	double* mu,
	double* nu,
	double* o,
	double* o_max,
	double* h,
	int* s)
{
	int n = n1 * n2 * n3;
	int i, j, k, m;
	float buf;
	int m0, m2, m4, m6,
		m24, m26, m46, m246;

	double k0, k1, k2;
	//------------------------------------------------------------------------------------------------
	//								ввод данных(начало)
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	//								начальные условия
	//------------------------------------------------------------------------------------------------

	for (m0 = 0; m0 < n; m0++)
	{
		u[m0] = 0;
		v[m0] = 0;
		w[m0] = 0;
		a[m0] = 0;
		mu[m0] = 1000;
		nu[m0] = 0.02;
	}

	for (j = 0; j < n2; j++)
		for (i = 0; i < n1; i++)
		{
			m = j + n2 * i;
			a1[m] = 0;
		}

	printf("nul OK\n");


	FILE* outs1;
	FILE* outs2;
	FILE* outs3;
	FILE* outs4;
	FILE* outs5;

	outs1 = fopen("setka1000.txt", "r+");
	outs2 = fopen("u.txt", "r+");
	outs3 = fopen("v.txt", "r+");
	outs4 = fopen("w.txt", "r+");
	outs5 = fopen("a.txt", "r+");



	for (k = 0; k < n3; k++)
		for (j = 0; j < n2; j++)
			for (i = 0; i < n1; i++)
			{
				m0 = k + n3 * j + n2 * n3 * i;

				o[m0] = 0;
				o_max[m0] = 0;
			}



	for (j = 0; j < n2 - 1; j++)
		for (i = 0; i < n1 - 1; i++)
		{
			m = j + n2 * i;

			fscanf(outs1, "%f", &buf);
			h[m] = buf;
		}


	for (k = 0; k < n3; k++)
		for (j = 0; j < n2; j++)
			for (i = 0; i < n1; i++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m = j + n2 * i;


				if (h[m] > 0)
				{
					o_max[m0] = 2 * h[m] + 6 - k;
					if (o_max[m0] > 1)
						o_max[m0] = 1;
					if (o_max[m0] < 0)
						o_max[m0] = 0;
					if (k < 6)
						o_max[m0] = 1;

					if (k > 5)
						o[m0] = o_max[m0];
				}

			}


	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m2 = m0 - n2 * n3;
				m4 = m0 - n3;
				m6 = m0 - 1;
				m24 = m0 - n2 * n3 - n3;
				m26 = m0 - n2 * n3 - 1;
				m46 = m0 - n3 - 1;
				m246 = m0 - n2 * n3 - n3 - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k0 = 0.5 * (k1 + k2);

				s[m0] = 0;
				if (k0 > 0)
					s[m0] = 1;


			}
	printf("h OK\n");




	for (k = 0; k < n3; k++)
		for (j = 0; j < n2; j++)
			for (i = 0; i < n1; i++)
			{
				m0 = k + n3 * j + n2 * n3 * i;

				//fscanf(outs2, "%f", &buf);
				//u[m0]=buf;
				//fscanf(outs3, "%f", &buf);
				//v[m0]=buf;
				//fscanf(outs4, "%f", &buf);
				//w[m0]=buf;
				//fscanf(outs5, "%f", &buf);
				//a[m0]=buf;

			}
	fclose(outs1);
	//fclose(outs2);
	//fclose(outs3);
	//fclose(outs4);
	//fclose(outs5);


	//------------------------------------------------------------------------------------------------
	//								граничные условия(конец)
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	//								ввод данных(конец)
	//------------------------------------------------------------------------------------------------
}





void vvodmask(double* u, double* v, double* w,
	double* a, double* o, double* o_max, int* s)

{
	int i, j, k;
	int m0, m2, m4, m5, m6,
		m24, m26, m46, m246, m15, m35, m135;

	double k0, k1, k2, k3, k4;
	double sum = 0;


	for (k = 1; k < n3 - 2; k++)
		for (j = 1; j < n2 - 2; j++)
			for (i = 1; i < n1 - 2; i++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m5 = m0 + 1;
				m15 = m0 + n2 * n3 + 1;
				m35 = m0 + n3 + 1;
				m135 = m0 + n2 * n3 + n3 + 1;


				o[m0] = (a[m5] + a[m15] + a[m35] + a[m135]) / (4 * po * g * dz) + k - 5;// ???????????????????????????????????????????????????????

				if (o[m0] > o_max[m0])
					o[m0] = o_max[m0];

				if (o[m0] < 0)
					o[m0] = 0;
			}





	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m2 = m0 - n2 * n3;
				m4 = m0 - n3;
				m6 = m0 - 1;
				m24 = m0 - n2 * n3 - n3;
				m26 = m0 - n2 * n3 - 1;
				m46 = m0 - n3 - 1;
				m246 = m0 - n2 * n3 - n3 - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k0 = 0.5 * (k1 + k2);

				if ((k0 > 0) && (s[m0] == 0))
					a[m0] = po * g * dz * (5 - k); // ???????????????????Не попадаем в отладке

				s[m0] = 0;
				if (k0 > 0)
					s[m0] = 1;
				if (k0 == 0)
				{
					a[m0] = 0;
					u[m0] = 0;
					v[m0] = 0;
					w[m0] = 0;
				}

			}

	j = 132;    // cиваш
	i = 1;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				
		sum = sum + k1 - k2;//???????????????????????????????????????????
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -115 / (dy * dz * sum);//???????????????????????????
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 6;    // гирла мертвый донец и средняя кутерьма
	i = 348;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -390 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 10;    // гирло кутерьма
	i = 347;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -424 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 16;    // гирло мокрая кутерьма
	i = 348;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -185 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 19;    // гирло мериновое
	i = 348;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -105 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 22;    // гирло песчаное
	i = 350;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -199 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 29;    // гирла свиное, кривое и богдан
	i = 353;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = -82 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 223;    // кубань
	i = 207;
	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;
		m2 = m0 - n2 * n3;
		m4 = m0 - n3;
		m6 = m0 - 1;
		m24 = m0 - n2 * n3 - n3;
		m26 = m0 - n2 * n3 - 1;
		m46 = m0 - n3 - 1;
		m246 = m0 - n2 * n3 - n3 - 1;

		k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
		k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);

		sum = sum + k4 - k3;
	}


	for (k = 1; k < n3; k++)
	{
		m0 = k + n3 * j + n2 * n3 * i;

		if (s[m0] > 0)
		{
			u[m0] = 0;
			v[m0] = -923 / (dx * dz * sum);
			s[m0] = 2;
		}
	}

	j = 231;
	for (i = 1; i < n1; i++)
		for (k = 1; k < n3; k++)
		{
			m0 = k + n3 * j + n2 * n3 * i;
			m2 = m0 - n2 * n3;
			m4 = m0 - n3;
			m6 = m0 - 1;
			m24 = m0 - n2 * n3 - n3;
			m26 = m0 - n2 * n3 - 1;
			m46 = m0 - n3 - 1;
			m246 = m0 - n2 * n3 - n3 - 1;

			k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
			k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
			k0 = 0.5 * (k3 + k4);

			if (k4 > k3)
			{
				s[m0] = 3;
			}
		}
}




void vivod(double* u, double* v, double* w, double* a)
{
	int i, j, k, m0;

	FILE* uoutfile;
	FILE* voutfile;
	FILE* woutfile;
	FILE* aoutfile;



	uoutfile = fopen("u.txt", "w+");
	voutfile = fopen("v.txt", "w+");
	woutfile = fopen("w.txt", "w+");
	aoutfile = fopen("a.txt", "w+");


	for (k = 0; k < n3; k++)
	{
		for (j = 0; j < n2; j++)
		{
			for (i = 0; i < n1; i++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				fprintf(uoutfile, "%f	", u[m0]);
				fprintf(voutfile, "%f	", v[m0]);
				fprintf(woutfile, "%f	", w[m0]);
				fprintf(aoutfile, "%f	", a[m0]);

			}
			fprintf(uoutfile, "\n");
			fprintf(voutfile, "\n");
			fprintf(woutfile, "\n");
			fprintf(aoutfile, "\n");

		}
	}
	fclose(uoutfile);
	fclose(voutfile);
	fclose(woutfile);
	fclose(aoutfile);

}

//------------------------------------------------------------------------------------------------
//										1
//									   11		 Расчет 
//									  1 1		скорости течения
//										1		на промежуточном
//										1		временном шаге.
//										1		(уравнение диффузии-
//									  11111		конвекции)
//------------------------------------------------------------------------------------------------

void firstfunc(double* u, double* v, double* w,
	double* mu, double* nu, double* o, int* s)
{
	double omega, sk;
	int i, j, k, it = 0;
	int m0, m1, m2, m3, m4, m5, m6,
		m24, m26, m46, m246;

	double k0, k1, k2, k3, k4, k5, k6;

	double* c0 = new double[n];
	double* c1 = new double[n];
	double* c2 = new double[n];
	double* c3 = new double[n];
	double* c4 = new double[n];
	double* c5 = new double[n];
	double* c6 = new double[n];

	double* f1 = new double[n];
	double* f2 = new double[n];
	double* f3 = new double[n];



	for (m0 = 0; m0 < n; m0++)
	{
		c1[m0] = 0;
		c2[m0] = 0;
		c3[m0] = 0;
		c4[m0] = 0;
		c5[m0] = 0;
		c6[m0] = 0;
	}
	printf("mass1 OK\n");
	//------------------------------------------------------------------------------------------------
	//								коэфициенты и правая часть
	//------------------------------------------------------------------------------------------------

	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m1 = m0 + n2 * n3;
				m2 = m0 - n2 * n3;
				m3 = m0 + n3;
				m4 = m0 - n3;
				m5 = m0 + 1;
				m6 = m0 - 1;
				m24 = m0 - n2 * n3 - n3;
				m26 = m0 - n2 * n3 - 1;
				m46 = m0 - n3 - 1;
				m246 = m0 - n2 * n3 - n3 - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
				k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
				k5 = 0.25 * (o[m0] + o[m2] + o[m4] + o[m24]);
				k6 = 0.25 * (o[m6] + o[m26] + o[m46] + o[m246]);
				k0 = 0.5 * (k1 + k2);


				c1[m0] = 0.5 * dt * (-(u[m1] + u[m0]) / 2 + (mu[m1] + mu[m0]) / dx) / dx;
				c2[m0] = 0.5 * dt * ( (u[m2] + u[m0]) / 2 + (mu[m2] + mu[m0]) / dx) / dx;
				c3[m0] = 0.5 * dt * (-(v[m3] + v[m0]) / 2 + (mu[m3] + mu[m0]) / dy) / dy;
				c4[m0] = 0.5 * dt * ( (v[m4] + v[m0]) / 2 + (mu[m4] + mu[m0]) / dy) / dy;
				c5[m0] = 0.5 * dt * (-(w[m5] + w[m0]) / 2 + (nu[m5] + nu[m0]) / dz) / dz;
				c6[m0] = 0.5 * dt * ( (w[m6] + w[m0]) / 2 + (nu[m6] + nu[m0]) / dz) / dz;


				if ((c1[m0] < 0) && (k1 > 0))
				{
					c1[m0] = 0.5 * dt * ((mu[m1] + mu[m0]) / dx) / dx;
					c2[m0] = 0.5 * dt * ((u[m2] + u[m0]) + (mu[m2] + mu[m0]) / dx) / dx;
				}
				if ((c2[m0] < 0) && (k2 > 0))
				{
					c1[m0] = 0.5 * dt * (-(u[m1] + u[m0]) + (mu[m1] + mu[m0]) / dx) / dx;
					c2[m0] = 0.5 * dt * ((mu[m2] + mu[m0]) / dx) / dx;
				}
				if ((c3[m0] < 0) && (k3 > 0))
				{
					c3[m0] = 0.5 * dt * ((mu[m3] + mu[m0]) / dy) / dy;
					c4[m0] = 0.5 * dt * ((v[m4] + v[m0]) + (mu[m4] + mu[m0]) / dy) / dy;
				}
				if ((c4[m0] < 0) && (k4 > 0))
				{
					c3[m0] = 0.5 * dt * (-(v[m3] + v[m0]) + (mu[m3] + mu[m0]) / dy) / dy;
					c4[m0] = 0.5 * dt * ((mu[m4] + mu[m0]) / dy) / dy;
				}
				if ((c5[m0] < 0) && (k5 > 0))
				{
					c5[m0] = 0.5 * dt * ((nu[m5] + nu[m0]) / dz) / dz;
					c6[m0] = 0.5 * dt * ((w[m6] + w[m0]) + (nu[m6] + nu[m0]) / dz) / dz;
				}
				if ((c6[m0] < 0) && (k6 > 0))
				{
					c5[m0] = 0.5 * dt * (-(w[m5] + w[m0]) + (nu[m5] + nu[m0]) / dz) / dz;
					c6[m0] = 0.5 * dt * ((nu[m6] + nu[m0]) / dz) / dz;
				}

				c1[m0] = k1 * c1[m0];
				c2[m0] = k2 * c2[m0];
				c3[m0] = k3 * c3[m0];
				c4[m0] = k4 * c4[m0];
				c5[m0] = k5 * c5[m0];
				c6[m0] = k6 * c6[m0];


				c0[m0] = k0 + c1[m0] + c3[m0] + c5[m0] + c2[m0] + c4[m0] + c6[m0];

				f1[m0] = k0 * (u[m0] + dt * kor * (v[m0] - w[m0]));
				f2[m0] = k0 * (v[m0] - dt * kor * u[m0]);
				f3[m0] = k0 * (w[m0] + dt * kor * u[m0]);


				if (k5 > k6)
				{
					sk = dt * ca * sqrt((sk_u - u[m0]) * (sk_u - u[m0]) + (sk_v - v[m0]) * (sk_v - v[m0]) + w[m0] * w[m0]);
					f1[m0] = f1[m0] + (k5 - k6) * sk * (sk_u) / dz;
					f2[m0] = f2[m0] + (k5 - k6) * sk * (sk_v) / dz;
					c0[m0] = c0[m0] + (k5 - k6) * sk / dz;

				}

				if (k6 > k5)
				{
					sk = dt * cv * sqrt(u[m0] * u[m0] + v[m0] * v[m0] + w[m0] * w[m0]) / dz;
					c0[m0] = c0[m0] + (k6 - k5) * sk;
				}


				if (s[m0] == 2)
				{
					c1[m0] = 0;
					c2[m0] = 0;
					c3[m0] = 0;
					c4[m0] = 0;
					c5[m0] = 0;
					c6[m0] = 0;


					c0[m0] = 1;

					f1[m0] = u[m0];
					f2[m0] = v[m0];
					f3[m0] = w[m0];

				}


			}
	printf("func1 OK\n");
	omega = 0;

	ptmss(u, c1, c2, c3, c4, c5, c6, c0, f1, s, omega, ev);
	ptmss(v, c1, c2, c3, c4, c5, c6, c0, f2, s, omega, ev);
	//ptmss(w,c1,c2,c3,c4,c5,c6,c0,f3,s,omega,ev);


	delete[] c0;
	delete[] c1;
	delete[] c2;
	delete[] c3;
	delete[] c4;
	delete[] c5;
	delete[] c6;

	delete[] f1;
	delete[] f2;
	delete[] f3;

}

//------------------------------------------------------------------------------------------------
//									   222
//									  2   2		 Рассчет
//									      2		давления 
//										 2		(уравнение
//										2		Пуассона)
//									   2		(Метод минимальных невязок)
//									  22222
//------------------------------------------------------------------------------------------------

void secondfunc(double* u, double* v, double* w, double* a, double* a1, double* o, int* s)
{

	int m, m0, m1, m2, m3, m4, m5, m6,
		m24, m26, m46, m246;

	double k0, k1, k2, k3, k4, k5, k6;
	double d0, d1, d2, d3;
	double omega;
	int i, j, k, it = 0;
	double* f = new double[n];

	double* c0 = new double[n];
	double* c1 = new double[n];
	double* c2 = new double[n];
	double* c3 = new double[n];
	double* c4 = new double[n];
	double* c5 = new double[n];
	double* c6 = new double[n];


	d0 = 2 / (dx * dx) + 2 / (dy * dy) + 2 / (dz * dz);
	d1 = 1 / (dx * dx);
	d2 = 1 / (dy * dy);
	d3 = 1 / (dz * dz);

	//------------------------------------------------------------------------------------------------
	//								правая часть
	//------------------------------------------------------------------------------------------------

	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m1 = m0 + n2 * n3;
				m2 = m0 - n2 * n3;
				m3 = m0 + n3;
				m4 = m0 - n3;
				m5 = m0 + 1;
				m6 = m0 - 1;
				m24 = m0 - n2 * n3 - n3;
				m26 = m0 - n2 * n3 - 1;
				m46 = m0 - n3 - 1;
				m246 = m0 - n2 * n3 - n3 - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
				k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
				k5 = 0.25 * (o[m0] + o[m2] + o[m4] + o[m24]);
				k6 = 0.25 * (o[m6] + o[m26] + o[m46] + o[m246]);
				k0 = 0.5 * (k1 + k2);



				c1[m0] = d1 * k1;
				c2[m0] = d1 * k2;
				c3[m0] = d2 * k3;
				c4[m0] = d2 * k4;
				c5[m0] = d3 * k5;
				c6[m0] = d3 * k6;


				c0[m0] = d1 * k1 + d1 * k2 + d2 * k3 + d2 * k4 + d3 * k5 + d3 * k6;


				f[m0] = -(po / dt) * (k1 * (u[m1] + u[m0]) / (2 * dx) - k2 * (u[m2] + u[m0]) / (2 * dx)
					+ k3 * (v[m3] + v[m0]) / (2 * dy) - k4 * (v[m4] + v[m0]) / (2 * dy)
					+ k5 * (w[m5] + w[m0]) / (2 * dz) - k6 * (w[m6] + w[m0]) / (2 * dz));


				if (k5 > k6)
				{
					c0[m0] = c0[m0] + (k5 - k6) / (dt * dt * dz * g);
					f[m0] = f[m0] + (k5 - k6) * a[m0] / (dt * dt * dz * g) - (po / dt) * isp * (k5 - k6);
				}

				if (s[m0] > 1)
					f[m0] = f[m0] - (po / dt) * ((k2 - k1) * u[m0] / dx + (k4 - k3) * v[m0] / dy);

			}

	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m = j + n2 * i;
				m0 = k + n3 * j + n2 * n3 * i;


				if (s[m0] > 0)
					a[m0] = a1[m];
			}

	printf("a2d OK\n");

	omega = 0;//1/(2*d0);

	//ptmss(a,c1,c2,c3,c4,c5,c6,c0,f,s,omega,ep);

	delete[] c0;
	delete[] c1;
	delete[] c2;
	delete[] c3;
	delete[] c4;
	delete[] c5;
	delete[] c6;


	delete[] f;

}


//------------------------------------------------------------------------------------------------
//									   333
//									  3   3		 Расчет
//									      3		скорости 
//									     3		на следующем
//										  3		временом шаге.
//								      3	  3		(явная схема)
//									   333
//------------------------------------------------------------------------------------------------


void thirdfunc(double* u, double* v, double* w, double* a,
	double* o, int* s)
{
	int i, j, k;
	int m0, m1, m2, m3, m4, m5, m6,
		m24, m26, m46, m246;

	double k0, k1, k2, k3, k4, k5, k6;



	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = k + n3 * j + n2 * n3 * i;
				m1 = m0 + n2 * n3;
				m2 = m0 - n2 * n3;
				m3 = m0 + n3;
				m4 = m0 - n3;
				m5 = m0 + 1;
				m6 = m0 - 1;
				m24 = m0 - n2 * n3 - n3;
				m26 = m0 - n2 * n3 - 1;
				m46 = m0 - n3 - 1;
				m246 = m0 - n2 * n3 - n3 - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
				k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
				k5 = 0.25 * (o[m0] + o[m2] + o[m4] + o[m24]);
				k6 = 0.25 * (o[m6] + o[m26] + o[m46] + o[m246]);
				k0 = 0.5 * (k1 + k2);

				if ((s[m0] > 0) && (s[m0] != 2))
				{
					u[m0] = u[m0] - dt * (k1 * (a[m1] - a[m0]) - k2 * (a[m2] - a[m0])) / (2 * dx * po * k0);
					v[m0] = v[m0] - dt * (k3 * (a[m3] - a[m0]) - k4 * (a[m4] - a[m0])) / (2 * dy * po * k0);
					//  w[m0]=w[m0]-dt*(k5*(a[m5]-a[m0])-k6*(a[m6]-a[m0]))/(2*dz*po*k0);
				}

			}
}




void ptmss(double* u, double* c1, double* c2, double* c3, double* c4,
	double* c5, double* c6, double* c0,
	double* f, int* s, double omega, double e)
{
	double max;
	double tay;
	int i, j, k, it = 0;
	int m0, m1, m2, m3, m4, m5, m6;


	tay = 2 * omega;
	double RwRw, ww, Aww, Awr, Rr;


	double* r = new double[n];

	printf("ptmss start OK\n");
	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(начало)
	//------------------------------------------------------------------------------------------------
	for (m0 = 0; m0 < n; m0++)
	{
		r[m0] = 0;

	}

	do
	{
		//------------------------------------------------------------------------------------------------
		//								Невязка
		//------------------------------------------------------------------------------------------------

		Aww = 0;
		RwRw = 0;
		ww = 0;
		max = 0;

		for (k = 1; k < n3 - 1; k++)
			for (i = 1; i < n1 - 1; i++)
				for (j = 1; j < n2 - 1; j++)
				{
					m0 = k + n3 * j + n2 * n3 * i;
					if (c0[m0] > 0)
					{
						m1 = m0 + n2 * n3;
						m2 = m0 - n2 * n3;
						m3 = m0 + n3;
						m4 = m0 - n3;
						m5 = m0 + 1;
						m6 = m0 - 1;


						r[m0] = f[m0] - c0[m0] * u[m0] + (c1[m0] * u[m1] + c2[m0] * u[m2]
							+ c3[m0] * u[m3] + c4[m0] * u[m4] + c5[m0] * u[m5] + c6[m0] * u[m6]);
						if (max < fabs(r[m0])) { max = fabs(r[m0]); };
					}
				}

		//------------------------------------------------------------------------------------------------
		//								Итерация поперемено треугольным методом
		//------------------------------------------------------------------------------------------------


		for (k = 1; k < n3 - 1; k++)
			for (i = 1; i < n1 - 1; i++)
				for (j = 1; j < n2 - 1; j++)
				{
					m0 = k + n3 * j + n2 * n3 * i;
					if (c0[m0] > 0)
					{
						m2 = m0 - n2 * n3;
						m4 = m0 - n3;
						m6 = m0 - 1;


						r[m0] = (omega * (c2[m0] * r[m2] + c4[m0] * r[m4] + c6[m0] * r[m6]) + r[m0]) / ((0.5 * omega + 1) * c0[m0]);
					}
				}


		for (k = n3 - 2; k > 0; k--)
			for (i = n1 - 2; i > 0; i--)
				for (j = n2 - 2; j > 0; j--)
				{
					m0 = k + n3 * j + n2 * n3 * i;
					if (c0[m0] > 0)
					{
						m1 = m0 + n2 * n3;
						m3 = m0 + n3;
						m5 = m0 + 1;


						r[m0] = (omega * (c1[m0] * r[m1] + c3[m0] * r[m3] + c5[m0] * r[m5]) + r[m0] * c0[m0]) / ((0.5 * omega + 1) * c0[m0]);
					}
				}


		for (k = 1; k < n3 - 1; k++)
			for (i = 1; i < n1 - 1; i++)
				for (j = 1; j < n2 - 1; j++)
				{
					m0 = k + n3 * j + n2 * n3 * i;
					if (c0[m0] > 0)
					{
						m1 = m0 + n2 * n3;
						m2 = m0 - n2 * n3;
						m3 = m0 + n3;
						m4 = m0 - n3;
						m5 = m0 + 1;
						m6 = m0 - 1;



						Awr = 1.1 * c0[m0] * r[m0] - (c1[m0] * r[m1] + c3[m0] * r[m3] + c5[m0] * r[m5] + c2[m0] * r[m2] + c4[m0] * r[m4] + c6[m0] * r[m6]);
						Rr = 0.5 * c0[m0] * r[m0] - (c1[m0] * r[m1] + c3[m0] * r[m3] + c5[m0] * r[m5]);

						RwRw = RwRw + Rr * Rr / c0[m0];
						Aww = Aww + Awr * r[m0];
						ww = ww + c0[m0] * r[m0] * r[m0];

					}
				}

		if (ww > 0)
		{
			tay = 2 * omega + ww / Aww;
			omega = sqrt(ww / RwRw);

		}

		//------------------------------------------------------------------------------------------------
		//								шаг ПТМ
		//------------------------------------------------------------------------------------------------


		for (m0 = 0; m0 < n; m0++)
			u[m0] = u[m0] + 1 * tay * r[m0];

		it++;

		printf("%f	%f	%f	%f	%d\n", t, max * 1000000, omega, tay, it);
	} while ((max > e) && (it < 300));

	printf("ptmss OK\n");

	delete[] r;


	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(конец)
	//------------------------------------------------------------------------------------------------

}




//------------------------------------------------------------------------------------------------
//									   222
//									  2   2		 ???????
//									      2		???????? 
//										 2		(?????????
//										2		????????)
//									   2		(????? ??????????? ???????)
//									  22222
//------------------------------------------------------------------------------------------------


void secondfunc2d(double* u, double* v, double* w, double* a, double* a1, double* o, int* s)
{

	int m, m0, m1, m2, m3, m4, m5, m6,
		m24, m26, m46, m246;

	double k0, k1, k2, k3, k4, k5, k6;

	double d0, d1, d2;
	double omega;
	int i, j, k, it = 0;
	double* f = new double[n2d];
	double* a2 = new double[n2d];

	double* c0 = new double[n2d];
	double* c1 = new double[n2d];
	double* c2 = new double[n2d];
	double* c3 = new double[n2d];
	double* c4 = new double[n2d];
	double* c5 = new double[n2d];
	double* c6 = new double[n2d];

	printf("mass2d OK\n");

	d0 = 2 / (dx * dx) + 2 / (dy * dy) + 2 / (dz * dz);
	d1 = 1 / (dx * dx);
	d2 = 1 / (dy * dy);

	//------------------------------------------------------------------------------------------------
	//								?????? ?????
	//------------------------------------------------------------------------------------------------
	for (m0 = 0; m0 < n2d; m0++)
	{
		c1[m0] = 0;
		c2[m0] = 0;
		c3[m0] = 0;
		c4[m0] = 0;


		c0[m0] = 0;

		f[m0] = 0;
		a2[m0] = a1[m0];

	}


	for (k = 1; k < n3 - 1; k++)
		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m = j + n2 * i;


				m0 = k + n3 * j + n2 * n3 * i;
				m1 = m0 + n2 * n3;
				m2 = m0 - n2 * n3;
				m3 = m0 + n3;
				m4 = m0 - n3;
				m5 = m0 + 1;
				m6 = m0 - 1;
				m24 = m0 - n2 * n3 - n3;
				m26 = m0 - n2 * n3 - 1;
				m46 = m0 - n3 - 1;
				m246 = m0 - n2 * n3 - n3 - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
				k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
				k5 = 0.25 * (o[m0] + o[m2] + o[m4] + o[m24]);
				k6 = 0.25 * (o[m6] + o[m26] + o[m46] + o[m246]);
				k0 = 0.5 * (k1 + k2);


				c1[m] = c1[m] + d1 * k1;
				c2[m] = c2[m] + d1 * k2;
				c3[m] = c3[m] + d2 * k3;
				c4[m] = c4[m] + d2 * k4;


				c0[m] = c0[m] + d1 * k1 + d1 * k2 + d2 * k3 + d2 * k4;


				f[m] = f[m] - (po / dt) * (k1 * (u[m1] + u[m0]) / (2 * dx) - k2 * (u[m2] + u[m0]) / (2 * dx)
					+ k3 * (v[m3] + v[m0]) / (2 * dy) - k4 * (v[m4] + v[m0]) / (2 * dy)
					+ k5 * (w[m5] + w[m0]) / (2 * dz) - k6 * (w[m6] + w[m0]) / (2 * dz));


				if (k5 > k6)
				{
					c0[m] = c0[m] + (k5 - k6) / (dt * dt * dz * g);
					f[m] = f[m] + (k5 - k6) * a[m0] / (dt * dt * dz * g) - (po / dt) * isp * (k5 - k6);
				}

				if (s[m0] > 1)
					f[m] = f[m] - (po / dt) * ((k2 - k1) * u[m0] / dx + (k4 - k3) * v[m0] / dy);

			}


	printf("func2d OK\n");

	omega = 0;//1/(2*d0);

	ptmgs2d(a1, c1, c2, c3, c4, c0, f, s, omega, ep);


	printf("ptm2d OK\n");




	delete[] c0;
	delete[] c1;
	delete[] c2;
	delete[] c3;
	delete[] c4;
	delete[] c5;
	delete[] c6;


	delete[] f;
	delete[] a2;

}

void ptmgs2d(double* u, double* c1, double* c2, double* c3, double* c4,
	double* c0, double* f, int* s, double omega, double e)
{
	double max;
	double tay;
	int i, j, it = 0;
	int m0, m1, m2, m3, m4;


	tay = 2 * omega;
	double RwRw, ww, Aww, Awr, Rr;


	double* r = new double[n2d];
	printf("r2d OK\n");

	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(начало)
	//------------------------------------------------------------------------------------------------

	do
	{
		//------------------------------------------------------------------------------------------------
		//								Невязка
		//------------------------------------------------------------------------------------------------

		Aww = 0;
		Awr = 0;
		RwRw = 0;
		ww = 0;

		for (i = 0; i < n1; i++)
			for (j = 0; j < n2; j++)
			{
				m0 = j + n2 * i;

				r[m0] = 0;

			}

		max = 0;

		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = j + n2 * i;
				m1 = m0 + n2;
				m2 = m0 - n2;
				m3 = m0 + 1;
				m4 = m0 - 1;



				r[m0] = f[m0] - c0[m0] * u[m0] + (c1[m0] * u[m1] + c2[m0] * u[m2]
					+ c3[m0] * u[m3] + c4[m0] * u[m4]);
				if (max < fabs(r[m0])) { max = fabs(r[m0]); };
			}

		//------------------------------------------------------------------------------------------------
		//								Итерация поперемено треугольным методом
		//------------------------------------------------------------------------------------------------


		for (i = 1; i < n1 - 1; i++)
			for (j = 1; j < n2 - 1; j++)
			{
				m0 = j + n2 * i;
				m1 = m0 + n2;
				m2 = m0 - n2;
				m3 = m0 + 1;
				m4 = m0 - 1;

				if (c0[m0] > 0)
				{
					r[m0] = (omega * (c2[m0] * r[m2] + c4[m0] * r[m4]) + r[m0]) / ((0.5 * omega + 1) * c0[m0]);
				}
			}

		for (i = n1 - 2; i > 0; i--)
			for (j = n2 - 2; j > 0; j--)
			{
				m0 = j + n2 * i;
				m1 = m0 + n2;
				m2 = m0 - n2;
				m3 = m0 + 1;
				m4 = m0 - 1;

				if (c0[m0] > 0)
				{
					r[m0] = (omega * (c1[m0] * r[m1] + c3[m0] * r[m3]) + r[m0] * c0[m0]) / ((0.5 * omega + 1) * c0[m0]);
				}
			}

		for (i = 1; i < n1 - 1; i++)

			for (j = 1; j < n2 - 1; j++)
			{
				m0 = j + n2 * i;
				m1 = m0 + n2;
				m2 = m0 - n2;
				m3 = m0 + 1;
				m4 = m0 - 1;

				if (c0[m0] > 0)
				{

					Awr = c0[m0] * r[m0] - (c1[m0] * r[m1] + c2[m0] * r[m2] + c3[m0] * r[m3] + c4[m0] * r[m4]);
					Rr = 0.5 * c0[m0] * r[m0] - (c1[m0] * r[m1] + c3[m0] * r[m3]);

					RwRw = RwRw + Rr * Rr / c0[m0];
					Aww = Aww + Awr * r[m0];
					ww = ww + c0[m0] * r[m0] * r[m0];
				}
			}

		if (ww > 0)
		{
			tay = 2 * omega + ww / Aww;
			omega = sqrt(ww / RwRw);
		}
		omega = 0;
		//------------------------------------------------------------------------------------------------
		//								шаг ПТМ
		//------------------------------------------------------------------------------------------------

		for (m0 = 0; m0 < n2d; m0++)
			u[m0] = u[m0] + tay * r[m0];

		it++;


		printf("%f	%f	%f	%f	%d\n", t, max * 1000000, omega, tay, it);
	} while ((max > e) && (it < 10000));


	delete[] r;


	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(конец)
	//------------------------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------------------------
//								Конец
//------------------------------------------------------------------------------------------------



void ptmsp(double* u, double* c1, double* c2, double* c3, double* c4,
	double* c5, double* c6, double* c01, double* c02, double* c0,
	double* f, int* s, double omega, double e)
{
	double max;
	double tay, alf = 1, b = 0;
	int i, j, k, it = 0;
	int m0, m1, m2, m3, m4, m5, m6;



	double AwAw, ww, Aww, Aw1w1, B1AwAw, B1AwAw1 = 1;


	double* r = new double[n];
	double* wr = new double[n];
	double* Awr = new double[n];
	double* Aksi = new double[n];
	double* ksi = new double[n];

	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(начало)
	//------------------------------------------------------------------------------------------------
	for (m0 = 0; m0 < n; m0++)
	{
		ksi[m0] = 0;
	}

	do
	{
		//------------------------------------------------------------------------------------------------
		//								Невязка
		//------------------------------------------------------------------------------------------------

		AwAw = 0;
		ww = 0;

		Aww = 0;

		B1AwAw = 0;

		for (m0 = 0; m0 < n; m0++)
		{
			r[m0] = 0;
			wr[m0] = 0;
			Awr[m0] = 0;
		}

		max = 0;

		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						r[m0] = f[m0] - c0[m0] * u[m0] + (c1[m0] * u[m1] + c2[m0] * u[m2]
							+ c3[m0] * u[m3] + c4[m0] * u[m4] + c5[m0] * u[m5] + c6[m0] * u[m6]);
						if (max < fabs(r[m0])) { max = fabs(r[m0]); };
					}
				}

		//------------------------------------------------------------------------------------------------
		//								Итерация поперемено треугольным методом
		//------------------------------------------------------------------------------------------------


		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m2 = m0 - (n2 + 1);
					m4 = m0 - 1;
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						wr[m0] = (omega * (c2[m0] * wr[m2] + c4[m0] * wr[m4] + c6[m0] * wr[m6]) + r[m0]) / (omega * c0[m0] / 2 + 1);
					}
				}

		for (k = n3 - 1; k > 0; k--)
			for (i = n1 - 1; i > 0; i--)
				for (j = n2 - 1; j > 0; j--)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m3 = m0 + 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						wr[m0] = (omega * (c1[m0] * wr[m1] + c3[m0] * wr[m3] + c5[m0] * wr[m5]) + wr[m0]) / (omega * c0[m0] / 2 + 1);
					}
				}

		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{

						Awr[m0] = c0[m0] * wr[m0] - (c1[m0] * wr[m1] + c2[m0] * wr[m2] + c3[m0] * wr[m3] + c4[m0] * wr[m4] + c5[m0] * wr[m5] + c6[m0] * wr[m6]);

						ww = ww + wr[m0] * wr[m0];
						AwAw = Aww + Awr[m0] * Awr[m0];

						Aww = Aww + Awr[m0] * wr[m0];
					}
				}

		if (it > 0)
			b = Aww / Aw1w1;

		Aw1w1 = Aww;

		for (m0 = 0; m0 < n; m0++)
		{
			ksi[m0] = wr[m0] + b * ksi[m0];
		}



		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{

						Aksi[m0] = c0[m0] * ksi[m0] - (c1[m0] * ksi[m1] + c2[m0] * ksi[m2] + c3[m0] * ksi[m3] + c4[m0] * ksi[m4] + c5[m0] * ksi[m5] + c6[m0] * ksi[m6]);

					}
				}


		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m2 = m0 - (n2 + 1);
					m4 = m0 - 1;
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						r[m0] = (omega * (c2[m0] * r[m2] + c4[m0] * r[m4] + c6[m0] * r[m6]) + Aksi[m0]) / (omega * c0[m0] / 2 + 1);
					}
				}

		for (k = n3 - 1; k > 0; k--)
			for (i = n1 - 1; i > 0; i--)
				for (j = n2 - 1; j > 0; j--)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m3 = m0 + 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						r[m0] = (omega * (c1[m0] * r[m1] + c3[m0] * r[m3] + c5[m0] * r[m5]) + r[m0]) / (omega * c0[m0] / 2 + 1);
						B1AwAw = B1AwAw + r[m0] * Aksi[m0];
					}
				}


		omega = 2 * sqrt(ww / AwAw);
		tay = Aww / B1AwAw;



		//------------------------------------------------------------------------------------------------
		//								шаг ПТМ
		//------------------------------------------------------------------------------------------------


		for (m0 = 0; m0 < n; m0++)
		{
			u[m0] = u[m0] + tay * ksi[m0];
		}

		it++;

		printf("%f	%f	%f	%f	%d\n", t, max * 1000000, omega, tay, it);
	} while ((max > e) && (it < 1000));

	delete[] r;
	delete[] wr;
	delete[] Awr;
	delete[] ksi;
	delete[] Aksi;

	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(конец)
	//------------------------------------------------------------------------------------------------

}



void ptmgs(double* u, double* c1, double* c2, double* c3, double* c4,
	double* c5, double* c6, double* c01, double* c02, double* c0,
	double* f, int* s, double omega, double e)
{
	double max;
	double tay;
	int i, j, k, it = 0;
	int m0, m1, m2, m3, m4, m5, m6;



	double BAwAw, BAw1Aw1, wBAw, Rwr, RBAwr, RwRBAw, Aww, ss, kk, rw, tetta;

	FILE* rez;

	rez = fopen("ss1.txt", "w+");

	double* r = new double[n];
	double* wr = new double[n];
	double* wr1 = new double[n];
	double* Awr = new double[n];
	double* Awr1 = new double[n];
	double* BAwr = new double[n];
	double* BAwr1 = new double[n];

	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(начало)
	//------------------------------------------------------------------------------------------------

	do
	{
		//------------------------------------------------------------------------------------------------
		//								Невязка
		//------------------------------------------------------------------------------------------------

		Aww = 0;
		BAwAw = 0;
		BAw1Aw1 = 0;
		ss = 1;
		kk = 0;
		rw = 0;
		tetta = 1;
		wBAw = 0;
		Rwr = 0;
		RBAwr = 0;
		RwRBAw = 0;

		for (m0 = 0; m0 < n; m0++)
		{
			r[m0] = 0;
			wr[m0] = 0;
			wr1[m0] = 0;
			Awr[m0] = 0;

			BAwr[m0] = 0;
			Awr1[m0] = 0;
			BAwr1[m0] = 0;
		}

		max = 0;

		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						r[m0] = f[m0] - c0[m0] * u[m0] + (c1[m0] * u[m1] + c2[m0] * u[m2]
							+ c3[m0] * u[m3] + c4[m0] * u[m4] + c5[m0] * u[m5] + c6[m0] * u[m6]);
						if (max < fabs(r[m0])) { max = fabs(r[m0]); };
					}
				}

		//------------------------------------------------------------------------------------------------
		//								Итерация поперемено треугольным методом
		//------------------------------------------------------------------------------------------------


		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						wr[m0] = (omega * (0.5 * (c2[m0] + c1[m2]) * wr[m2] + 0.5 * (c4[m0] + c3[m4]) * wr[m4] + 0.5 * (c6[m0] + c5[m6]) * wr[m6]) + r[m0]) / (0.5 * omega * c0[m0] + 1);
					}
				}

		for (k = n3 - 1; k > 0; k--)
			for (i = n1 - 1; i > 0; i--)
				for (j = n2 - 1; j > 0; j--)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						wr[m0] = (omega * (0.5 * (c1[m0] + c2[m1]) * wr[m1] + 0.5 * (c3[m0] + c4[m3]) * wr[m3] + 0.5 * (c5[m0] + c6[m5]) * wr[m5]) + wr[m0]) / (0.5 * omega * c0[m0] + 1);
					}
				}

		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{

						Awr[m0] = c0[m0] * wr[m0] - (0.5 * (c1[m0] + c2[m1]) * wr[m1] + 0.5 * (c2[m0] + c1[m2]) * wr[m2] + 0.5 * (c3[m0] + c4[m3]) * wr[m3] +
							0.5 * (c4[m0] + c3[m4]) * wr[m4] + 0.5 * (c5[m0] + c6[m5]) * wr[m5] + 0.5 * (c6[m0] + c5[m6]) * wr[m6]);
						wr1[m0] = c0[m0] * r[m0] - (0.5 * (c1[m0] + c2[m1]) * r[m1] + 0.5 * (c2[m0] + c1[m2]) * r[m2] + 0.5 * (c3[m0] + c4[m3]) * r[m3] +
							0.5 * (c4[m0] + c3[m4]) * r[m4] + 0.5 * (c5[m0] + c6[m5]) * r[m5] + 0.5 * (c6[m0] + c5[m6]) * r[m6]);
						Awr1[m0] = -(0.5 * (c1[m0] - c2[m1]) * wr[m1] + 0.5 * (c2[m0] - c1[m2]) * wr[m2] + 0.5 * (c3[m0] - c4[m3]) * wr[m3] +
							0.5 * (c4[m0] - c3[m4]) * wr[m4] + 0.5 * (c5[m0] - c6[m5]) * wr[m5] + 0.5 * (c6[m0] - c5[m6]) * wr[m6]);
					}
				}

		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						BAwr[m0] = (omega * (0.5 * (c2[m0] + c1[m2]) * BAwr[m2] + 0.5 * (c4[m0] + c3[m4]) * BAwr[m4] + 0.5 * (c6[m0] + c5[m6]) * BAwr[m6]) + Awr[m0]) / (0.5 * omega * c0[m0] + 1);
						BAwr1[m0] = (omega * (0.5 * (c2[m0] + c1[m2]) * BAwr1[m2] + 0.5 * (c4[m0] + c3[m4]) * BAwr1[m4] + 0.5 * (c6[m0] + c5[m6]) * BAwr1[m6]) + Awr1[m0]) / (0.5 * omega * c0[m0] + 1);
					}
				}

		for (k = n3 - 1; k > 0; k--)
			for (i = n1 - 1; i > 0; i--)
				for (j = n2 - 1; j > 0; j--)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						BAwr[m0] = (omega * (0.5 * (c1[m0] + c2[m1]) * BAwr[m1] + 0.5 * (c3[m0] + c4[m3]) * BAwr[m3] + 0.5 * (c5[m0] + c6[m5]) * BAwr[m5]) + BAwr[m0]) / (0.5 * omega * c0[m0] + 1);
						BAwr1[m0] = (omega * (0.5 * (c1[m0] + c2[m1]) * BAwr1[m1] + 0.5 * (c3[m0] + c4[m3]) * BAwr1[m3] + 0.5 * (c5[m0] + c6[m5]) * BAwr1[m5]) + BAwr1[m0]) / (0.5 * omega * c0[m0] + 1);

					}
				}

		for (k = 1; k < n3; k++)
			for (i = 1; i < n1; i++)
				for (j = 1; j < n2; j++)
				{
					m0 = j + (n2 + 1) * (i + (n1 + 1) * k);
					m1 = m0 + (n2 + 1);
					m2 = m0 - (n2 + 1);
					m3 = m0 + 1;
					m4 = m0 - 1;
					m5 = m0 + (n2 + 1) * (n1 + 1);
					m6 = m0 - (n2 + 1) * (n1 + 1);

					if (s[m0] != 0)
					{
						wBAw = wBAw + wr[m0] * BAwr[m0];
						Rwr = 0.5 * c0[m0] * wr[m0] - (0.5 * (c2[m0] + c1[m2]) * wr[m2] +
							0.5 * (c4[m0] + c3[m4]) * wr[m4] + 0.5 * (c6[m0] + c5[m6]) * wr[m6]);
						RBAwr = 0.5 * c0[m0] * BAwr[m0] - (0.5 * (c2[m0] + c1[m2]) * BAwr[m2] +
							0.5 * (c4[m0] + c3[m4]) * BAwr[m4] + 0.5 * (c6[m0] + c5[m6]) * BAwr[m6]);
						RwRBAw = RwRBAw + Rwr * RBAwr;

						rw = rw + r[m0] * wr[m0];
						Aww = Aww + Awr[m0] * wr[m0];
						BAwAw = BAwAw + BAwr[m0] * Awr[m0];
						BAw1Aw1 = BAw1Aw1 + BAwr1[m0] * Awr1[m0];
					}
				}



		omega = 0;
		if ((wBAw / RwRBAw) > 0)
			omega = sqrt(fabs(wBAw / RwRBAw));
		tay = Aww / BAwAw;
		ss = sqrt(1 - Aww * Aww / (BAwAw * rw));
		kk = (BAw1Aw1 / rw) * (Aww / BAwAw) * (Aww / BAwAw);
		tetta = (1 - ss * sqrt(kk / (1 + kk - ss * ss))) / (1 + kk);
		tay = tay * tetta;



		//------------------------------------------------------------------------------------------------
		//								шаг ПТМ
		//------------------------------------------------------------------------------------------------


		for (m0 = 0; m0 < n; m0++)
			u[m0] = u[m0] + tay * wr[m0];

		it++;

		printf("%f	%f	%f	%f	%d\n", t, max * 1000000, omega, tetta, it);
		fprintf(rez, "%f	%f	%f	%f	%f	%f	%f	%d\n", t, max * 1000000, kk, ss, tetta, omega, tay, it);
	} while ((max > e) && (it < 200));

	fclose(rez);

	getch();

	delete[] r;
	delete[] wr;
	delete[] wr1;
	delete[] Awr;
	delete[] BAwr;
	delete[] Awr1;
	delete[] BAwr1;
	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(конец)
	//------------------------------------------------------------------------------------------------

}
