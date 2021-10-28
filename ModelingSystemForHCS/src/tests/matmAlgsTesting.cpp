#include "testFunctions.h"

void algStart(void (*algFunPntr)(LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, double),
	LinearArray3D* r, LinearArray3D* c0, LinearArray3D* c2, LinearArray3D* c4, LinearArray3D* c6, LinearArray3D* s, double w,
	LinearArray3D* arrayForVerification, int numberOfLaunches)
{
	timer<std::chrono::microseconds> aTimer;

	SimpleStatistics statistics{};

	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		aTimer.start();
		algFunPntr(r, c0, c2, c4, c6, s, w);
		auto elapsed = aTimer.stop();
		bool isEqual = arrayForVerification->IsEqual(r);
		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}

	statistics.print();
}

// alg1
void alg1(LinearArray3D* r, LinearArray3D* c0, LinearArray3D* c2, LinearArray3D* c4, LinearArray3D* c6, LinearArray3D* s, double w)
{
	int nx = r->nx;
	int ny = r->ny;
	int nz = r->nz;

	for (int k = 1; k < nz - 1; k++)
	{
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++)
			{
				double s0 = s->GetElement(i, j, k);
				if (s0 > (1 - 0.001) && s0 < (1 + 0.001))
				{
					double rm0 = r->GetElement(i, j, k);
					double rm2 = r->GetElement(i - 1, j, k);
					double rm4 = r->GetElement(i, j - 1, k);
					double rm6 = r->GetElement(i, j, k - 1);

					double c0m0 = c0->GetElement(i, j, k);
					double c2m0 = c2->GetElement(i, j, k);
					double c4m0 = c4->GetElement(i, j, k);
					double c6m0 = c6->GetElement(i, j, k);

					double newVal = (w * (c2m0 * rm2 + c4m0 * rm4 + c6m0 * rm6) + rm0) / (w * c0m0 / 2 + 1);
					r->SetElement(i, j, k, newVal);
				}
			}
		}
	}
}

// alg2
void alg2(LinearArray3D* r, LinearArray3D* c0, LinearArray3D* c2, LinearArray3D* c4, LinearArray3D* c6, LinearArray3D* s, double w)
{
	int nx = r->nx;
	int ny = r->ny;
	int nz = r->nz;

	for (int k = 1; k < nz - 1; k++)
	{
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++)
			{
				double s0 = s->GetElement(i, j, k);
				if (s0 > (1 - 0.001) && s0 < (1 + 0.001))
				{
					double rm0 = r->GetElement(i, j, k);
					double rm2 = r->GetElement(i - 1, j, k);
					double rm4 = r->GetElement(i, j - 1, k);
					double rm6 = r->GetElement(i, j, k - 1);

					double c0m0 = c0->GetElement(i, j, k);
					double c2m0 = c2->GetElement(i, j, k);
					double c4m0 = c4->GetElement(i, j, k);
					double c6m0 = c6->GetElement(i, j, k);

					double newVal = (w * (c2m0 * rm2 + c4m0 * rm4 + c6m0 * rm6) + rm0) / (w * c0m0 / 2 + 1);
					r->SetElement(i, j, k, newVal);
				}
			}
		}
	}
}

//// alg3
//void alg3(LinearArray3D* linAr)
//{
//	auto nx = linAr->nx;
//	auto ny = linAr->ny;
//	auto nz = linAr->nz;
//
//	auto nxy = nx * ny;
//
//	for (auto k = 1; k < nz - 1; k++)
//	{
//		auto zOffset = k * nx * ny;
//		for (auto j = 1; j < ny - 1; j++)
//		{
//			auto yOffset = j * nx;
//			auto yzOffset = zOffset + yOffset;
//			for (auto i = 1; i < nx - 1; i++)
//			{
//				int m0 = i + yzOffset;
//				int m2 = m0 - 1;
//				int m4 = m0 - nx;
//				int m6 = m0 - nxy;
//
//				double val2 = linAr->data[m2];
//				double val0 = linAr->data[m0];
//
//				double val4 = linAr->data[m4];
//				double val6 = linAr->data[m6];
//				double newVal = val0 + val2 + val4 + val6;
//				linAr->SetElement(i, j, k, newVal);
//			}
//		}
//	}
//}
//
//// alg4
//void alg4(LinearArray3D* linAr)
//{
//	int nx = linAr->nx;
//	int ny = linAr->ny;
//	int nz = linAr->nz;
//
//	//1. Выделяем буферы для m2+m0, m4, m6
//	//double* b20 = (double*)malloc(nx * sizeof(double));
//	//double* b4 = (double*)malloc(nx * sizeof(double));
//	//double* b6 = (double*)malloc(nx * sizeof(double));
//	LinearArray1D* b20 = new LinearArray1D(nx);
//	LinearArray1D* b4 = new LinearArray1D(nx);
//	LinearArray1D* b6 = new LinearArray1D(nx);
//
//	for (size_t k = 1; k < nz - 1; k++)
//	{
//		for (size_t j = 1; j < ny - 1; j++)
//		{
//			//2. Загрузка данных для буферов (start: b20, b4, b6; next: b20, b6) 
//			linAr->GetLineX(j, k, b20);
//			if (j == 1)
//			{
//				linAr->GetLineX(j - 1, k, b4);
//			}
//			linAr->GetLineX(j, k - 1, b6);
//			/*b20->Print();
//			b4->Print();
//			b6->Print();*/
//
//			//3. Расчет для тек. буферов
//			for (size_t i = 1; i < nx - 1; i++)
//			{
//				double _b2 = b20->data[i - 1];
//				double _b0 = b20->data[i];
//				double _b4 = b4->data[i];
//				double _b6 = b6->data[i];
//				double _res = _b0 + _b2 + _b4 + _b6;
//				//b20->data[i] = b20->data[i] + b20->data[i - 1] + b4->data[i] + b6->data[i];
//				b20->data[i] = _res;
//			}
//			/*b20->Print();
//			b4->Print();
//			b6->Print();*/
//
//			//5. Сохранение b20
//			linAr->SetLineX(j, k, b20);
//			//linAr->Print();
//
//			//4. Перестановка указателя с b20 на b4
//			LinearArray1D* tmp = b4;
//			b4 = b20;
//			b20 = tmp;
//
//			/*b20->Print();
//			b4->Print();
//			b6->Print();*/
//		}
//	}
//}
//
//// alg5
//void alg5(LinearArray3D* linAr)
//{
//	size_t nx = linAr->nx;
//	size_t ny = linAr->ny;
//	size_t nz = linAr->nz;
//
//	auto layerZprev = (double*)malloc(nx * ny * sizeof(double));
//	auto layerZcurr = (double*)malloc(nx * ny * sizeof(double));
//	linAr->GetLayerZ(0, layerZprev);
//
//	/*for (size_t j = 0; j < ny; j++)
//	{
//		for (size_t i = 0; i < nx; i++)
//		{
//			std::cout << layerZprev[i + j*nx] << " ";
//		}
//		std::cout << std::endl;
//	}		*/
//
//	for (int k = 1; k < nz - 1; k++)
//	{
//		linAr->GetLayerZ(k, layerZcurr);
//		for (int j = 1; j < ny - 1; j++)
//		{
//			for (int i = 1; i < nx - 1; i++)
//			{
//				int m0 = i + j * nx;
//				int m2 = m0 - 1;
//				int m4 = m0 - nx;
//				int m6 = m0;
//
//				double val2 = layerZcurr[m2];
//				double val0 = layerZcurr[m0];
//
//				double val4 = layerZcurr[m4];
//				double val6 = layerZprev[m6];
//				double newVal = val0 + val2 + val4 + val6;
//				layerZcurr[m0] = newVal;
//			}
//		}
//
//		linAr->SetLayerZ(k, layerZcurr);
//
//		double* tmp = layerZprev;
//		layerZprev = layerZcurr;
//		layerZcurr = tmp;
//	}
//}
//
//// alg6
//void alg6(LinearArray3D* linAr)
//{
//	size_t nx = linAr->nx;
//	size_t ny = linAr->ny;
//	size_t nz = linAr->nz;
//
//	auto layerZprev = new LinearArray2D(nx, ny);
//	auto layerZcurr = new LinearArray2D(nx, ny);
//	linAr->GetLayerZ(0, layerZprev);
//
//	for (int k = 1; k < nz - 1; k++)
//	{
//		linAr->GetLayerZ(k, layerZcurr);
//		for (int j = 1; j < ny - 1; j++)
//		{
//			for (int i = 1; i < nx - 1; i++)
//			{
//				int m0 = i + j * nx;
//				int m2 = m0 - 1;
//				int m4 = m0 - nx;
//				int m6 = m0;
//
//				double val2 = layerZcurr->data[m2];
//				double val0 = layerZcurr->data[m0];
//
//				double val4 = layerZcurr->data[m4];
//				double val6 = layerZprev->data[m6];
//				double newVal = val0 + val2 + val4 + val6;
//				layerZcurr->data[m0] = newVal;
//			}
//		}
//
//		linAr->SetLayerZ(k, layerZcurr);
//
//		auto tmp = layerZprev;
//		layerZprev = layerZcurr;
//		layerZcurr = tmp;
//	}
//}
//
//// alg7
//void alg7(LinearArray3D* linAr)
//{
//	size_t nx = linAr->nx;
//	size_t ny = linAr->ny;
//	size_t nz = linAr->nz;
//
//	auto layerZprev = new LinearArray2D(nx, ny);
//	auto layerZcurr = new LinearArray2D(nx, ny);
//	linAr->GetLayerZ(0, layerZprev);
//
//	LinearArray1D* b20 = new LinearArray1D(nx);
//	LinearArray1D* b4 = new LinearArray1D(nx);
//	//LinearArray1D* b6  = new LinearArray1D(nx);
//
//	for (int k = 1; k < nz - 1; k++)
//	{
//		linAr->GetLayerZ(k, layerZcurr);
//		layerZcurr->GetLineX(0, b4);
//		for (int j = 1; j < ny - 1; j++)
//		{
//			layerZcurr->GetLineX(j, b20);
//			//layerZprev->GetLineX(j, b6);
//			for (int i = 1; i < nx - 1; i++)
//			{
//				double val2 = b20->data[i - 1];
//				double val0 = b20->data[i];
//
//				double val4 = b4->data[i];
//				double val6 = layerZprev->data[i + j * nx];
//				double newVal = val0 + val2 + val4 + val6;
//				b20->data[i] = newVal;
//			}
//
//			//Сохранение b20
//			layerZcurr->SetLineX(j, b20);
//
//			//Перестановка указателя с b20 на b4
//			auto* tmp = b4;
//			b4 = b20;
//			b20 = tmp;
//		}
//
//		linAr->SetLayerZ(k, layerZcurr);
//
//		auto tmp = layerZprev;
//		layerZprev = layerZcurr;
//		layerZcurr = tmp;
//	}
//}


void matmAlgsTesting()
{
	int nx = 100;
	int ny = 50;
	int nz = 40;
	auto r = new LinearArray3D(nx, ny, nz);
	auto c0 = new LinearArray3D(nx, ny, nz);
	auto c2 = new LinearArray3D(nx, ny, nz);
	auto c4 = new LinearArray3D(nx, ny, nz);
	auto c6 = new LinearArray3D(nx, ny, nz);
	auto s = new LinearArray3D(nx, ny, nz);
	double w = 0.5;
	int numberOfLaunches = 10;// Количество запусков алгоритма

	std::cout << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "-------- Running test: matmAlgsTesting() --------" << std::endl;
	std::cout << "nx = " << nx << std::endl;
	std::cout << "ny = " << ny << std::endl;
	std::cout << "nz = " << nz << std::endl;

	// Инициализация массивов
	r->InitLinearArray3DByValue(10);
	c0->InitLinearArray3DByValue(1);
	c2->InitLinearArray3DByValue(2);
	c4->InitLinearArray3DByValue(4);
	c6->InitLinearArray3DByValue(6);
	s->InitLinearArray3DByValue(1);

	// Массив для верификации
	auto arrayForVerification = new LinearArray3D(nx, ny, nz);
	arrayForVerification->InitLinearArray3DByValue(10);
	alg1(arrayForVerification, c0, c2, c4, c6, s, w);
	//arrayForVerification->Print();
	///////////////////////////////////////////////////////////

	std::cout << "---alg1---\n";
	r->InitLinearArray3DByValue(10);
	algStart(alg1, r, c0, c2, c4, c6, s, w, arrayForVerification, numberOfLaunches);

	std::cout << "---alg2---\n";
	r->InitLinearArray3DByValue(10);
	algStart(alg2, r, c0, c2, c4, c6, s, w, arrayForVerification, numberOfLaunches);
}
