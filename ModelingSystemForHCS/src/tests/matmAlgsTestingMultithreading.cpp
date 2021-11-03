#include "testFunctions.h"


void algStart_9(void (*algFunPntr)(LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, LinearArray3D*, double),
	LinearArray3D* r_t0, LinearArray3D* c0_t0, LinearArray3D* c2_t0, LinearArray3D* c4_t0, LinearArray3D* c6_t0, LinearArray3D* s_t0,
	LinearArray3D* r_t1, LinearArray3D* c0_t1, LinearArray3D* c2_t1, LinearArray3D* c4_t1, LinearArray3D* c6_t1, LinearArray3D* s_t1,
	LinearArray3D* r_t2, LinearArray3D* c0_t2, LinearArray3D* c2_t2, LinearArray3D* c4_t2, LinearArray3D* c6_t2, LinearArray3D* s_t2,
	LinearArray3D* r_t3, LinearArray3D* c0_t3, LinearArray3D* c2_t3, LinearArray3D* c4_t3, LinearArray3D* c6_t3, LinearArray3D* s_t3,
	LinearArray3D* r_t4, LinearArray3D* c0_t4, LinearArray3D* c2_t4, LinearArray3D* c4_t4, LinearArray3D* c6_t4, LinearArray3D* s_t4,
	LinearArray3D* r_t5, LinearArray3D* c0_t5, LinearArray3D* c2_t5, LinearArray3D* c4_t5, LinearArray3D* c6_t5, LinearArray3D* s_t5,
	LinearArray3D* r_t6, LinearArray3D* c0_t6, LinearArray3D* c2_t6, LinearArray3D* c4_t6, LinearArray3D* c6_t6, LinearArray3D* s_t6,
	LinearArray3D* r_t7, LinearArray3D* c0_t7, LinearArray3D* c2_t7, LinearArray3D* c4_t7, LinearArray3D* c6_t7, LinearArray3D* s_t7,
	LinearArray3D* r_t8, LinearArray3D* c0_t8, LinearArray3D* c2_t8, LinearArray3D* c4_t8, LinearArray3D* c6_t8, LinearArray3D* s_t8,
	LinearArray3D* r_t9, LinearArray3D* c0_t9, LinearArray3D* c2_t9, LinearArray3D* c4_t9, LinearArray3D* c6_t9, LinearArray3D* s_t9,
	LinearArray3D* r_t10, LinearArray3D* c0_t10, LinearArray3D* c2_t10, LinearArray3D* c4_t10, LinearArray3D* c6_t10, LinearArray3D* s_t10,
	LinearArray3D* r_t11, LinearArray3D* c0_t11, LinearArray3D* c2_t11, LinearArray3D* c4_t11, LinearArray3D* c6_t11, LinearArray3D* s_t11,
	LinearArray3D* r_t12, LinearArray3D* c0_t12, LinearArray3D* c2_t12, LinearArray3D* c4_t12, LinearArray3D* c6_t12, LinearArray3D* s_t12,
	LinearArray3D* r_t13, LinearArray3D* c0_t13, LinearArray3D* c2_t13, LinearArray3D* c4_t13, LinearArray3D* c6_t13, LinearArray3D* s_t13,
	LinearArray3D* r_t14, LinearArray3D* c0_t14, LinearArray3D* c2_t14, LinearArray3D* c4_t14, LinearArray3D* c6_t14, LinearArray3D* s_t14,
	LinearArray3D* r_t15, LinearArray3D* c0_t15, LinearArray3D* c2_t15, LinearArray3D* c4_t15, LinearArray3D* c6_t15, LinearArray3D* s_t15,
	double w, LinearArray3D* arrayForVerification, int numberOfLaunches)
{
	timer<std::chrono::microseconds> aTimer;

	SimpleStatistics statistics{};

	// Последовательное выполнение в основном потоке
	std::cout << "--- Sequancial: Main thread ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		aTimer.start();
		algFunPntr(r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);		
		bool isEqual = isEqual_0;		

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Последовательное выполнение в новом потоке
	std::cout << "--- Sequancial: New thread ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		aTimer.start();	
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);		
		t0.join();
		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);		
		bool isEqual = isEqual_0;		

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 2 потока
	std::cout << "--- Parallel: 2 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		aTimer.start();		
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);		
		t0.join();
		t1.join();
		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);		
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;		

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 3 потока
	std::cout << "--- Parallel: 3 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);

		aTimer.start();		
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);		

		t0.join();
		t1.join();
		t2.join();		

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;		

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 4 потока
	std::cout << "--- Parallel: 4 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);

		aTimer.start();		
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		t0.join();
		t1.join();
		t2.join();
		t3.join();
		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;		

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 5 потоков
	std::cout << "--- Parallel: 5 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);

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
	statistics.data.clear();

	// Параллельное выполнение в 6 потоков
	std::cout << "--- Parallel: 6 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);

		aTimer.start();		
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 7 потоков
	std::cout << "--- Parallel: 7 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 8 потоков
	std::cout << "--- Parallel: 8 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 9 потоков
	std::cout << "--- Parallel: 9 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 10 потоков
	std::cout << "--- Parallel: 10 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 11 потоков
	std::cout << "--- Parallel: 11 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);
		r_t10->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);
		std::thread t10(algFunPntr, r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		t10.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual_10 = arrayForVerification->IsEqual(r_t10);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;
		isEqual = isEqual && isEqual_10;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 12 потоков
	std::cout << "--- Parallel: 12 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);
		r_t10->InitLinearArray3DByValue(10);
		r_t11->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);
		std::thread t10(algFunPntr, r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10, w);
		std::thread t11(algFunPntr, r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		t10.join();
		t11.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual_10 = arrayForVerification->IsEqual(r_t10);
		bool isEqual_11 = arrayForVerification->IsEqual(r_t11);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;
		isEqual = isEqual && isEqual_10;
		isEqual = isEqual && isEqual_11;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 13 потоков
	std::cout << "--- Parallel: 13 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);
		r_t10->InitLinearArray3DByValue(10);
		r_t11->InitLinearArray3DByValue(10);
		r_t12->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);
		std::thread t10(algFunPntr, r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10, w);
		std::thread t11(algFunPntr, r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11, w);
		std::thread t12(algFunPntr, r_t12, c0_t12, c2_t12, c4_t12, c6_t12, s_t12, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		t10.join();
		t11.join();
		t12.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual_10 = arrayForVerification->IsEqual(r_t10);
		bool isEqual_11 = arrayForVerification->IsEqual(r_t11);
		bool isEqual_12 = arrayForVerification->IsEqual(r_t12);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;
		isEqual = isEqual && isEqual_10;
		isEqual = isEqual && isEqual_11;
		isEqual = isEqual && isEqual_12;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 14 потоков
	std::cout << "--- Parallel: 14 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);
		r_t10->InitLinearArray3DByValue(10);
		r_t11->InitLinearArray3DByValue(10);
		r_t12->InitLinearArray3DByValue(10);
		r_t13->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);
		std::thread t10(algFunPntr, r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10, w);
		std::thread t11(algFunPntr, r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11, w);
		std::thread t12(algFunPntr, r_t12, c0_t12, c2_t12, c4_t12, c6_t12, s_t12, w);
		std::thread t13(algFunPntr, r_t13, c0_t13, c2_t13, c4_t13, c6_t13, s_t13, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		t10.join();
		t11.join();
		t12.join();
		t13.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual_10 = arrayForVerification->IsEqual(r_t10);
		bool isEqual_11 = arrayForVerification->IsEqual(r_t11);
		bool isEqual_12 = arrayForVerification->IsEqual(r_t12);
		bool isEqual_13 = arrayForVerification->IsEqual(r_t13);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;
		isEqual = isEqual && isEqual_10;
		isEqual = isEqual && isEqual_11;
		isEqual = isEqual && isEqual_12;
		isEqual = isEqual && isEqual_13;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 15 потоков
	std::cout << "--- Parallel: 15 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);
		r_t10->InitLinearArray3DByValue(10);
		r_t11->InitLinearArray3DByValue(10);
		r_t12->InitLinearArray3DByValue(10);
		r_t13->InitLinearArray3DByValue(10);
		r_t14->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);
		std::thread t10(algFunPntr, r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10, w);
		std::thread t11(algFunPntr, r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11, w);
		std::thread t12(algFunPntr, r_t12, c0_t12, c2_t12, c4_t12, c6_t12, s_t12, w);
		std::thread t13(algFunPntr, r_t13, c0_t13, c2_t13, c4_t13, c6_t13, s_t13, w);
		std::thread t14(algFunPntr, r_t14, c0_t14, c2_t14, c4_t14, c6_t14, s_t14, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		t10.join();
		t11.join();
		t12.join();
		t13.join();
		t14.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual_10 = arrayForVerification->IsEqual(r_t10);
		bool isEqual_11 = arrayForVerification->IsEqual(r_t11);
		bool isEqual_12 = arrayForVerification->IsEqual(r_t12);
		bool isEqual_13 = arrayForVerification->IsEqual(r_t13);
		bool isEqual_14 = arrayForVerification->IsEqual(r_t14);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;
		isEqual = isEqual && isEqual_10;
		isEqual = isEqual && isEqual_11;
		isEqual = isEqual && isEqual_12;
		isEqual = isEqual && isEqual_13;
		isEqual = isEqual && isEqual_14;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();

	// Параллельное выполнение в 16 потоков
	std::cout << "--- Parallel: 16 threads ---" << std::endl;
	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		r_t0->InitLinearArray3DByValue(10);
		r_t1->InitLinearArray3DByValue(10);
		r_t2->InitLinearArray3DByValue(10);
		r_t3->InitLinearArray3DByValue(10);
		r_t4->InitLinearArray3DByValue(10);
		r_t5->InitLinearArray3DByValue(10);
		r_t6->InitLinearArray3DByValue(10);
		r_t7->InitLinearArray3DByValue(10);
		r_t8->InitLinearArray3DByValue(10);
		r_t9->InitLinearArray3DByValue(10);
		r_t10->InitLinearArray3DByValue(10);
		r_t11->InitLinearArray3DByValue(10);
		r_t12->InitLinearArray3DByValue(10);
		r_t13->InitLinearArray3DByValue(10);
		r_t14->InitLinearArray3DByValue(10);
		r_t15->InitLinearArray3DByValue(10);

		aTimer.start();
		std::thread t0(algFunPntr, r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
		std::thread t1(algFunPntr, r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1, w);
		std::thread t2(algFunPntr, r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2, w);
		std::thread t3(algFunPntr, r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3, w);
		std::thread t4(algFunPntr, r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4, w);
		std::thread t5(algFunPntr, r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5, w);
		std::thread t6(algFunPntr, r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6, w);
		std::thread t7(algFunPntr, r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7, w);
		std::thread t8(algFunPntr, r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8, w);
		std::thread t9(algFunPntr, r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t9, w);
		std::thread t10(algFunPntr, r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10, w);
		std::thread t11(algFunPntr, r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11, w);
		std::thread t12(algFunPntr, r_t12, c0_t12, c2_t12, c4_t12, c6_t12, s_t12, w);
		std::thread t13(algFunPntr, r_t13, c0_t13, c2_t13, c4_t13, c6_t13, s_t13, w);
		std::thread t14(algFunPntr, r_t14, c0_t14, c2_t14, c4_t14, c6_t14, s_t14, w);
		std::thread t15(algFunPntr, r_t15, c0_t15, c2_t15, c4_t15, c6_t15, s_t15, w);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		t10.join();
		t11.join();
		t12.join();
		t13.join();
		t14.join();
		t15.join();

		auto elapsed = aTimer.stop();

		bool isEqual_0 = arrayForVerification->IsEqual(r_t0);
		bool isEqual_1 = arrayForVerification->IsEqual(r_t1);
		bool isEqual_2 = arrayForVerification->IsEqual(r_t2);
		bool isEqual_3 = arrayForVerification->IsEqual(r_t3);
		bool isEqual_4 = arrayForVerification->IsEqual(r_t4);
		bool isEqual_5 = arrayForVerification->IsEqual(r_t5);
		bool isEqual_6 = arrayForVerification->IsEqual(r_t6);
		bool isEqual_7 = arrayForVerification->IsEqual(r_t7);
		bool isEqual_8 = arrayForVerification->IsEqual(r_t8);
		bool isEqual_9 = arrayForVerification->IsEqual(r_t9);
		bool isEqual_10 = arrayForVerification->IsEqual(r_t10);
		bool isEqual_11 = arrayForVerification->IsEqual(r_t11);
		bool isEqual_12 = arrayForVerification->IsEqual(r_t12);
		bool isEqual_13 = arrayForVerification->IsEqual(r_t13);
		bool isEqual_14 = arrayForVerification->IsEqual(r_t14);
		bool isEqual_15 = arrayForVerification->IsEqual(r_t15);
		bool isEqual = isEqual_0;
		isEqual = isEqual && isEqual_1;
		isEqual = isEqual && isEqual_2;
		isEqual = isEqual && isEqual_3;
		isEqual = isEqual && isEqual_4;
		isEqual = isEqual && isEqual_5;
		isEqual = isEqual && isEqual_6;
		isEqual = isEqual && isEqual_7;
		isEqual = isEqual && isEqual_8;
		isEqual = isEqual && isEqual_9;
		isEqual = isEqual && isEqual_10;
		isEqual = isEqual && isEqual_11;
		isEqual = isEqual && isEqual_12;
		isEqual = isEqual && isEqual_13;
		isEqual = isEqual && isEqual_14;
		isEqual = isEqual && isEqual_15;

		if (isEqual) statistics.add(elapsed.get_time_as_double());
	}
	statistics.print();
	statistics.data.clear();
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



// alg3p4_without_s
void alg3p4_without_s_9(LinearArray3D* r, LinearArray3D* c0, LinearArray3D* c2, LinearArray3D* c4, LinearArray3D* c6, LinearArray3D* s, double w)
{
	auto nx = r->nx;
	auto ny = r->ny;
	auto nz = r->nz;

	auto nxy = nx * ny;

	int m0 = 0;
	int m2 = 0;
	int m4 = 0;
	int m6 = 0;
	double rm2 = 0;
	double rm0 = 0;
	double rm4 = 0;
	double rm6 = 0;
	double c0m0 = 0;
	double c2m0 = 0;
	double c4m0 = 0;
	double c6m0 = 0;
	double newVal = 0;

	int m0p1 = 0;
	int m2p1 = 0;
	int m4p1 = 0;
	int m6p1 = 0;
	double rm2p1 = 0;
	double rm0p1 = 0;
	double rm4p1 = 0;
	double rm6p1 = 0;
	double c0m0p1 = 0;
	double c2m0p1 = 0;
	double c4m0p1 = 0;
	double c6m0p1 = 0;
	double newValp1 = 0;

	int m0p2 = 0;
	int m2p2 = 0;
	int m4p2 = 0;
	int m6p2 = 0;
	double rm2p2 = 0;
	double rm0p2 = 0;
	double rm4p2 = 0;
	double rm6p2 = 0;
	double c0m0p2 = 0;
	double c2m0p2 = 0;
	double c4m0p2 = 0;
	double c6m0p2 = 0;
	double newValp2 = 0;

	int m0p3 = 0;
	int m2p3 = 0;
	int m4p3 = 0;
	int m6p3 = 0;
	double rm2p3 = 0;
	double rm0p3 = 0;
	double rm4p3 = 0;
	double rm6p3 = 0;
	double c0m0p3 = 0;
	double c2m0p3 = 0;
	double c4m0p3 = 0;
	double c6m0p3 = 0;
	double newValp3 = 0;

	int m0p4 = 0;
	int m2p4 = 0;
	int m4p4 = 0;
	int m6p4 = 0;
	double rm2p4 = 0;
	double rm0p4 = 0;
	double rm4p4 = 0;
	double rm6p4 = 0;
	double c0m0p4 = 0;
	double c2m0p4 = 0;
	double c4m0p4 = 0;
	double c6m0p4 = 0;
	double newValp4 = 0;

	int xStep = 5; // !CHANGE
	int xGroups = nx / xStep;
	int xLast = nx % xStep;
	if (xLast == 0)
	{
		xGroups--;
	}

	for (auto k = 1; k < nz - 1; k++)
	{
		auto zOffset = k * nx * ny;
		for (auto j = 1; j < ny - 1; j++)
		{
			auto yOffset = j * nx;
			auto yzOffset = zOffset + yOffset;

			m0 = yzOffset;
			m2 = m0 - 1;
			m4 = m0 - nx;
			m6 = m0 - nxy;

			for (auto i = 1; i < xStep; i++)
			{
				m0++;
				m2++;
				m4++;
				m6++;

				rm2 = r->data[m2];
				rm0 = r->data[m0];
				rm4 = r->data[m4];
				rm6 = r->data[m6];

				c0m0 = c0->data[m0];
				c2m0 = c2->data[m0];
				c4m0 = c4->data[m0];
				c6m0 = c6->data[m0];

				newVal = (w * (c2m0 * rm2 + c4m0 * rm4 + c6m0 * rm6) + rm0) / (w * c0m0 / 2 + 1);
				r->data[m0] = newVal;
			}

			for (auto xGroup = 1; xGroup < xGroups; xGroup++)
			{
				m0 = xGroup * xStep + yzOffset;
				m0p1 = m0 + 1;
				m0p2 = m0 + 2;
				m0p3 = m0 + 3;
				m0p4 = m0 + 4;

				m2 = m0 - 1;
				m2p1 = m2 + 1;
				m2p2 = m2 + 2;
				m2p3 = m2 + 3;
				m2p4 = m2 + 4;

				m4 = m0 - nx;
				m4p1 = m4 + 1;
				m4p2 = m4 + 2;
				m4p3 = m4 + 3;
				m4p4 = m4 + 4;

				m6 = m0 - nxy;
				m6p1 = m6 + 1;
				m6p2 = m6 + 2;
				m6p3 = m6 + 3;
				m6p4 = m6 + 4;

				rm2 = r->data[m2];
				rm0 = r->data[m0];
				rm4 = r->data[m4];
				rm6 = r->data[m6];

				c0m0 = c0->data[m0];
				c2m0 = c2->data[m0];
				c4m0 = c4->data[m0];
				c6m0 = c6->data[m0];

				newVal = (w * (c2m0 * rm2 + c4m0 * rm4 + c6m0 * rm6) + rm0) / (w * c0m0 / 2 + 1);
				r->data[m0] = newVal;

				////////////////

				//rm2p1 = r->data[m2p1];
				rm2p1 = newVal;
				rm0p1 = r->data[m0p1];
				rm4p1 = r->data[m4p1];
				rm6p1 = r->data[m6p1];

				c0m0p1 = c0->data[m0p1];
				c2m0p1 = c2->data[m0p1];
				c4m0p1 = c4->data[m0p1];
				c6m0p1 = c6->data[m0p1];

				newValp1 = (w * (c2m0p1 * rm2p1 + c4m0p1 * rm4p1 + c6m0p1 * rm6p1) + rm0p1) / (w * c0m0p1 / 2 + 1);
				r->data[m0p1] = newValp1;

				////////////////

				//rm2p2 = r->data[m2p2];
				rm2p2 = newValp1;
				rm0p2 = r->data[m0p2];
				rm4p2 = r->data[m4p2];
				rm6p2 = r->data[m6p2];

				c0m0p2 = c0->data[m0p2];
				c2m0p2 = c2->data[m0p2];
				c4m0p2 = c4->data[m0p2];
				c6m0p2 = c6->data[m0p2];

				newValp2 = (w * (c2m0p2 * rm2p2 + c4m0p2 * rm4p2 + c6m0p2 * rm6p2) + rm0p2) / (w * c0m0p2 / 2 + 1);
				r->data[m0p2] = newValp2;

				////////////////

				//rm2p3 = r->data[m2p3];
				rm2p3 = newValp2;
				rm0p3 = r->data[m0p3];
				rm4p3 = r->data[m4p3];
				rm6p3 = r->data[m6p3];

				c0m0p3 = c0->data[m0p3];
				c2m0p3 = c2->data[m0p3];
				c4m0p3 = c4->data[m0p3];
				c6m0p3 = c6->data[m0p3];

				newValp3 = (w * (c2m0p3 * rm2p3 + c4m0p3 * rm4p3 + c6m0p3 * rm6p3) + rm0p3) / (w * c0m0p3 / 2 + 1);
				r->data[m0p3] = newValp3;

				////////////////

				//rm2p4 = r->data[m2p4];
				rm2p4 = newValp3;
				rm0p4 = r->data[m0p4];
				rm4p4 = r->data[m4p4];
				rm6p4 = r->data[m6p4];

				c0m0p4 = c0->data[m0p4];
				c2m0p4 = c2->data[m0p4];
				c4m0p4 = c4->data[m0p4];
				c6m0p4 = c6->data[m0p4];

				newValp4 = (w * (c2m0p4 * rm2p4 + c4m0p4 * rm4p4 + c6m0p4 * rm6p4) + rm0p4) / (w * c0m0p4 / 2 + 1);// ! CHANGE ..p4
				r->data[m0p4] = newValp4;
			}

			for (auto il = xGroups * xStep; il < nx - 1; il++)
			{
				m0 = il + yzOffset;
				m2 = m0 - 1;
				m4 = m0 - nx;
				m6 = m0 - nxy;

				rm2 = r->data[m2];
				rm0 = r->data[m0];
				rm4 = r->data[m4];
				rm6 = r->data[m6];

				c0m0 = c0->data[m0];
				c2m0 = c2->data[m0];
				c4m0 = c4->data[m0];
				c6m0 = c6->data[m0];

				newVal = (w * (c2m0 * rm2 + c4m0 * rm4 + c6m0 * rm6) + rm0) / (w * c0m0 / 2 + 1);
				r->data[m0] = newVal;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/// <summary>
/// Исследование зависимости времени расчета одного шага ПТМ в многопоточном режиме
/// </summary>
void matmAlgsTestingMultithreading()
{
	std::cout << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "-------- Running test: matmAlgsTestingMultithreading() --------" << std::endl;
		
	/*int i = 1000;
	int j = 100;
	int k = 100;*/

	for (size_t k = 50; k <= 200; k += 50)
	{
		for (size_t j = 50; j <= 200; j += 50)
		{
			for (size_t i = 50; i <= 1000; i += 50)
			{
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

				auto r_t5 = new LinearArray3D(nx, ny, nz);
				auto c0_t5 = new LinearArray3D(nx, ny, nz);
				auto c2_t5 = new LinearArray3D(nx, ny, nz);
				auto c4_t5 = new LinearArray3D(nx, ny, nz);
				auto c6_t5 = new LinearArray3D(nx, ny, nz);
				auto s_t5 = new LinearArray3D(nx, ny, nz);

				auto r_t6 = new LinearArray3D(nx, ny, nz);
				auto c0_t6 = new LinearArray3D(nx, ny, nz);
				auto c2_t6 = new LinearArray3D(nx, ny, nz);
				auto c4_t6 = new LinearArray3D(nx, ny, nz);
				auto c6_t6 = new LinearArray3D(nx, ny, nz);
				auto s_t6 = new LinearArray3D(nx, ny, nz);

				auto r_t7 = new LinearArray3D(nx, ny, nz);
				auto c0_t7 = new LinearArray3D(nx, ny, nz);
				auto c2_t7 = new LinearArray3D(nx, ny, nz);
				auto c4_t7 = new LinearArray3D(nx, ny, nz);
				auto c6_t7 = new LinearArray3D(nx, ny, nz);
				auto s_t7 = new LinearArray3D(nx, ny, nz);

				auto r_t8 = new LinearArray3D(nx, ny, nz);
				auto c0_t8 = new LinearArray3D(nx, ny, nz);
				auto c2_t8 = new LinearArray3D(nx, ny, nz);
				auto c4_t8 = new LinearArray3D(nx, ny, nz);
				auto c6_t8 = new LinearArray3D(nx, ny, nz);
				auto s_t8 = new LinearArray3D(nx, ny, nz);

				auto r_t9 = new LinearArray3D(nx, ny, nz);
				auto c0_t9 = new LinearArray3D(nx, ny, nz);
				auto c2_t9 = new LinearArray3D(nx, ny, nz);
				auto c4_t9 = new LinearArray3D(nx, ny, nz);
				auto c6_t9 = new LinearArray3D(nx, ny, nz);
				auto s_t9 = new LinearArray3D(nx, ny, nz);

				auto r_t10 = new LinearArray3D(nx, ny, nz);
				auto c0_t10 = new LinearArray3D(nx, ny, nz);
				auto c2_t10 = new LinearArray3D(nx, ny, nz);
				auto c4_t10 = new LinearArray3D(nx, ny, nz);
				auto c6_t10 = new LinearArray3D(nx, ny, nz);
				auto s_t10 = new LinearArray3D(nx, ny, nz);

				auto r_t11 = new LinearArray3D(nx, ny, nz);
				auto c0_t11 = new LinearArray3D(nx, ny, nz);
				auto c2_t11 = new LinearArray3D(nx, ny, nz);
				auto c4_t11 = new LinearArray3D(nx, ny, nz);
				auto c6_t11 = new LinearArray3D(nx, ny, nz);
				auto s_t11 = new LinearArray3D(nx, ny, nz);

				auto r_t12 = new LinearArray3D(nx, ny, nz);
				auto c0_t12 = new LinearArray3D(nx, ny, nz);
				auto c2_t12 = new LinearArray3D(nx, ny, nz);
				auto c4_t12 = new LinearArray3D(nx, ny, nz);
				auto c6_t12 = new LinearArray3D(nx, ny, nz);
				auto s_t12 = new LinearArray3D(nx, ny, nz);

				auto r_t13 = new LinearArray3D(nx, ny, nz);
				auto c0_t13 = new LinearArray3D(nx, ny, nz);
				auto c2_t13 = new LinearArray3D(nx, ny, nz);
				auto c4_t13 = new LinearArray3D(nx, ny, nz);
				auto c6_t13 = new LinearArray3D(nx, ny, nz);
				auto s_t13 = new LinearArray3D(nx, ny, nz);

				auto r_t14 = new LinearArray3D(nx, ny, nz);
				auto c0_t14 = new LinearArray3D(nx, ny, nz);
				auto c2_t14 = new LinearArray3D(nx, ny, nz);
				auto c4_t14 = new LinearArray3D(nx, ny, nz);
				auto c6_t14 = new LinearArray3D(nx, ny, nz);
				auto s_t14 = new LinearArray3D(nx, ny, nz);

				auto r_t15 = new LinearArray3D(nx, ny, nz);
				auto c0_t15 = new LinearArray3D(nx, ny, nz);
				auto c2_t15 = new LinearArray3D(nx, ny, nz);
				auto c4_t15 = new LinearArray3D(nx, ny, nz);
				auto c6_t15 = new LinearArray3D(nx, ny, nz);
				auto s_t15 = new LinearArray3D(nx, ny, nz);

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

				r_t5->InitLinearArray3DByValue(10);
				c0_t5->InitLinearArray3DByValue(1);
				c2_t5->InitLinearArray3DByValue(2);
				c4_t5->InitLinearArray3DByValue(4);
				c6_t5->InitLinearArray3DByValue(6);
				s_t5->InitLinearArray3DByValue(1);

				r_t6->InitLinearArray3DByValue(10);
				c0_t6->InitLinearArray3DByValue(1);
				c2_t6->InitLinearArray3DByValue(2);
				c4_t6->InitLinearArray3DByValue(4);
				c6_t6->InitLinearArray3DByValue(6);
				s_t6->InitLinearArray3DByValue(1);

				r_t7->InitLinearArray3DByValue(10);
				c0_t7->InitLinearArray3DByValue(1);
				c2_t7->InitLinearArray3DByValue(2);
				c4_t7->InitLinearArray3DByValue(4);
				c6_t7->InitLinearArray3DByValue(6);
				s_t7->InitLinearArray3DByValue(1);

				r_t8->InitLinearArray3DByValue(10);
				c0_t8->InitLinearArray3DByValue(1);
				c2_t8->InitLinearArray3DByValue(2);
				c4_t8->InitLinearArray3DByValue(4);
				c6_t8->InitLinearArray3DByValue(6);
				s_t8->InitLinearArray3DByValue(1);

				r_t9->InitLinearArray3DByValue(10);
				c0_t9->InitLinearArray3DByValue(1);
				c2_t9->InitLinearArray3DByValue(2);
				c4_t9->InitLinearArray3DByValue(4);
				c6_t9->InitLinearArray3DByValue(6);
				s_t9->InitLinearArray3DByValue(1);

				r_t10->InitLinearArray3DByValue(10);
				c0_t10->InitLinearArray3DByValue(1);
				c2_t10->InitLinearArray3DByValue(2);
				c4_t10->InitLinearArray3DByValue(4);
				c6_t10->InitLinearArray3DByValue(6);
				s_t10->InitLinearArray3DByValue(1);

				r_t11->InitLinearArray3DByValue(10);
				c0_t11->InitLinearArray3DByValue(1);
				c2_t11->InitLinearArray3DByValue(2);
				c4_t11->InitLinearArray3DByValue(4);
				c6_t11->InitLinearArray3DByValue(6);
				s_t11->InitLinearArray3DByValue(1);

				r_t12->InitLinearArray3DByValue(10);
				c0_t12->InitLinearArray3DByValue(1);
				c2_t12->InitLinearArray3DByValue(2);
				c4_t12->InitLinearArray3DByValue(4);
				c6_t12->InitLinearArray3DByValue(6);
				s_t12->InitLinearArray3DByValue(1);

				r_t13->InitLinearArray3DByValue(10);
				c0_t13->InitLinearArray3DByValue(1);
				c2_t13->InitLinearArray3DByValue(2);
				c4_t13->InitLinearArray3DByValue(4);
				c6_t13->InitLinearArray3DByValue(6);
				s_t13->InitLinearArray3DByValue(1);

				r_t14->InitLinearArray3DByValue(10);
				c0_t14->InitLinearArray3DByValue(1);
				c2_t14->InitLinearArray3DByValue(2);
				c4_t14->InitLinearArray3DByValue(4);
				c6_t14->InitLinearArray3DByValue(6);
				s_t14->InitLinearArray3DByValue(1);

				r_t15->InitLinearArray3DByValue(10);
				c0_t15->InitLinearArray3DByValue(1);
				c2_t15->InitLinearArray3DByValue(2);
				c4_t15->InitLinearArray3DByValue(4);
				c6_t15->InitLinearArray3DByValue(6);
				s_t15->InitLinearArray3DByValue(1);

				// Массив для верификации
				auto arrayForVerification = new LinearArray3D(nx, ny, nz);
				arrayForVerification->InitLinearArray3DByValue(10);
				alg1_9(arrayForVerification, c0_t0, c2_t0, c4_t0, c6_t0, s_t0, w);
				//arrayForVerification->Print();
				///////////////////////////////////////////////////////////

				std::cout << "\n------------------------------------------------------------------\n";
				std::cout << "---alg1---\n";				
				algStart_9(alg1_9,
					r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0,
					r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1,
					r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2,
					r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3,
					r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4,
					r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5,
					r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6,
					r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7,
					r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8,
					r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t4,
					r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10,
					r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11,
					r_t12, c0_t12, c2_t12, c4_t12, c6_t12, s_t12,
					r_t13, c0_t13, c2_t13, c4_t13, c6_t13, s_t13,
					r_t14, c0_t14, c2_t14, c4_t14, c6_t14, s_t14,
					r_t15, c0_t15, c2_t15, c4_t15, c6_t15, s_t15,
					w, arrayForVerification, numberOfLaunches);

				std::cout << "\n------------------------------------------------------------------\n";
				std::cout << "---alg3p4_without_s---\n";
				algStart_9(alg3p4_without_s_9,
					r_t0, c0_t0, c2_t0, c4_t0, c6_t0, s_t0,
					r_t1, c0_t1, c2_t1, c4_t1, c6_t1, s_t1,
					r_t2, c0_t2, c2_t2, c4_t2, c6_t2, s_t2,
					r_t3, c0_t3, c2_t3, c4_t3, c6_t3, s_t3,
					r_t4, c0_t4, c2_t4, c4_t4, c6_t4, s_t4,
					r_t5, c0_t5, c2_t5, c4_t5, c6_t5, s_t5,
					r_t6, c0_t6, c2_t6, c4_t6, c6_t6, s_t6,
					r_t7, c0_t7, c2_t7, c4_t7, c6_t7, s_t7,
					r_t8, c0_t8, c2_t8, c4_t8, c6_t8, s_t8,
					r_t9, c0_t9, c2_t9, c4_t9, c6_t9, s_t4,
					r_t10, c0_t10, c2_t10, c4_t10, c6_t10, s_t10,
					r_t11, c0_t11, c2_t11, c4_t11, c6_t11, s_t11,
					r_t12, c0_t12, c2_t12, c4_t12, c6_t12, s_t12,
					r_t13, c0_t13, c2_t13, c4_t13, c6_t13, s_t13,
					r_t14, c0_t14, c2_t14, c4_t14, c6_t14, s_t14,
					r_t15, c0_t15, c2_t15, c4_t15, c6_t15, s_t15,
					w, arrayForVerification, numberOfLaunches);
			}
		}
	}
}