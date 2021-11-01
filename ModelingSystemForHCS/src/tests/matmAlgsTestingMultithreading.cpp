#include "testFunctions.h"


void algStart_9(void (*algFunPntr)(LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, double),
	LinearArray3D* r_t0, LinearArray3D* c0_t0, LinearArray3D* c2_t0, LinearArray3D* c4_t0, LinearArray3D* c6_t0, LinearArray3D* s_t0,
	LinearArray3D* r_t1, LinearArray3D* c0_t1, LinearArray3D* c2_t1, LinearArray3D* c4_t1, LinearArray3D* c6_t1, LinearArray3D* s_t1,
	LinearArray3D* r_t2, LinearArray3D* c0_t2, LinearArray3D* c2_t2, LinearArray3D* c4_t2, LinearArray3D* c6_t2, LinearArray3D* s_t2,
	LinearArray3D* r_t3, LinearArray3D* c0_t3, LinearArray3D* c2_t3, LinearArray3D* c4_t3, LinearArray3D* c6_t3, LinearArray3D* s_t3,
	LinearArray3D* r_t4, LinearArray3D* c0_t4, LinearArray3D* c2_t4, LinearArray3D* c4_t4, LinearArray3D* c6_t4, LinearArray3D* s_t4,
	double w, LinearArray3D* arrayForVerification, int numberOfLaunches)
{
	timer<std::chrono::microseconds> aTimer;

	SimpleStatistics statistics{};

	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		aTimer.start();
		//algFunPntr(r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		//algFunPntr(r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}

	statistics.print();
}

// alg1
void alg1_9(LinearArray3D* r, LinearArray3D* c0, LinearArray3D* c2, LinearArray3D* c4, LinearArray3D* c6, LinearArray3D* s, double w)
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

/// <summary>
/// Исследование зависимости времени расчета одного шага ПТМ в многопоточном режиме
/// </summary>
void matmAlgsTestingMultithreading()
{
	std::cout << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "-------- Running test: matmAlgsTestingMultithreading() --------" << std::endl;
		
	int i = 100;
	int j = 100;
	int k = 100;

	int nx = i;
	int ny = j;
	int nz = k;
	auto r_t0 = new LinearArray3D(nx, ny, nz);
	auto c0_t0 = new LinearArray3D(nx, ny, nz);
	auto c2_t0 = new LinearArray3D(nx, ny, nz);
	auto c4_t0 = new LinearArray3D(nx, ny, nz);
	auto c6_t0 = new LinearArray3D(nx, ny, nz);
	auto s_t0 = new LinearArray3D(nx, ny, nz);

	auto r_t1 = new LinearArray3D(nx, ny, nz);
	auto c0_t1 = new LinearArray3D(nx, ny, nz);
	auto c2_t1 = new LinearArray3D(nx, ny, nz);
	auto c4_t1 = new LinearArray3D(nx, ny, nz);
	auto c6_t1 = new LinearArray3D(nx, ny, nz);
	auto s_t1 = new LinearArray3D(nx, ny, nz);

	auto r_t2 = new LinearArray3D(nx, ny, nz);
	auto c0_t2 = new LinearArray3D(nx, ny, nz);
	auto c2_t2 = new LinearArray3D(nx, ny, nz);
	auto c4_t2 = new LinearArray3D(nx, ny, nz);
	auto c6_t2 = new LinearArray3D(nx, ny, nz);
	auto s_t2 = new LinearArray3D(nx, ny, nz);

	auto r_t3 = new LinearArray3D(nx, ny, nz);
	auto c0_t3 = new LinearArray3D(nx, ny, nz);
	auto c2_t3 = new LinearArray3D(nx, ny, nz);
	auto c4_t3 = new LinearArray3D(nx, ny, nz);
	auto c6_t3 = new LinearArray3D(nx, ny, nz);
	auto s_t3 = new LinearArray3D(nx, ny, nz);

	auto r_t4 = new LinearArray3D(nx, ny, nz);
	auto c0_t4 = new LinearArray3D(nx, ny, nz);
	auto c2_t4 = new LinearArray3D(nx, ny, nz);
	auto c4_t4 = new LinearArray3D(nx, ny, nz);
	auto c6_t4 = new LinearArray3D(nx, ny, nz);
	auto s_t4 = new LinearArray3D(nx, ny, nz);

	double w = 0.5;
	int numberOfLaunches = 10;// Количество запусков алгоритма

	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "nx = " << nx << std::endl;
	std::cout << "ny = " << ny << std::endl;
	std::cout << "nz = " << nz << std::endl;

	// Инициализация массивов
	r_t0->InitLinearArray3DByValue(10);
	c0_t0->InitLinearArray3DByValue(1);
	c2_t0->InitLinearArray3DByValue(2);
	c4_t0->InitLinearArray3DByValue(4);
	c6_t0->InitLinearArray3DByValue(6);
	s_t0->InitLinearArray3DByValue(1);

	r_t1->InitLinearArray3DByValue(10);
	c0_t1->InitLinearArray3DByValue(1);
	c2_t1->InitLinearArray3DByValue(2);
	c4_t1->InitLinearArray3DByValue(4);
	c6_t1->InitLinearArray3DByValue(6);
	s_t1->InitLinearArray3DByValue(1);

	r_t2->InitLinearArray3DByValue(10);
	c0_t2->InitLinearArray3DByValue(1);
	c2_t2->InitLinearArray3DByValue(2);
	c4_t2->InitLinearArray3DByValue(4);
	c6_t2->InitLinearArray3DByValue(6);
	s_t2->InitLinearArray3DByValue(1);

	r_t3->InitLinearArray3DByValue(10);
	c0_t3->InitLinearArray3DByValue(1);
	c2_t3->InitLinearArray3DByValue(2);
	c4_t3->InitLinearArray3DByValue(4);
	c6_t3->InitLinearArray3DByValue(6);
	s_t3->InitLinearArray3DByValue(1);

	r_t4->InitLinearArray3DByValue(10);
	c0_t4->InitLinearArray3DByValue(1);
	c2_t4->InitLinearArray3DByValue(2);
	c4_t4->InitLinearArray3DByValue(4);
	c6_t4->InitLinearArray3DByValue(6);
	s_t4->InitLinearArray3DByValue(1);

	// Массив для верификации
	auto arrayForVerification = new LinearArray3D(nx, ny, nz);
	arrayForVerification->InitLinearArray3DByValue(10);
	alg1_9(arrayForVerification, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
	//arrayForVerification->Print();
	///////////////////////////////////////////////////////////

	std::cout << "---alg1---\n";
	r_t0->InitLinearArray3DByValue(10);
	algStart_9(alg1_9,
		r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0,
		r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1,
		r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2,
		r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3,
		r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4,
		w, arrayForVerification, numberOfLaunches);
}